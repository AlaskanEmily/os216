/*  Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 * - Products derived from this software may not be called "os216", nor may
 *     "216" appear in their name, without prior written permission of
 *     the copyright holders.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "os216_sized_malloc.h"

#include "os216_nano_fatal.h"
#include "os216_nano_bitmap.h"
#include "os216_nano_physmem.h"
#include "os216_synchro_spinlock.h"

#include <stdlib.h>

#define OS216_SIZED_MALLOC_ALIGN 4

#define OS216_ALIGN_SIZE(N, ALIGN) (((N) + (ALIGN) - 1) / (ALIGN))

#define OS216_PAGE_ALIGN_SIZE(N) OS216_ALIGN_SIZE(N, os216_nano_page_size)

#define OS216_SMALL_ALIGN_SIZE(N) OS216_ALIGN_SIZE(N, OS216_SIZED_MALLOC_ALIGN)

/* Lock around the small pool. */
static os216_spinlock_t os216_small_pool_lock = OS216_SPINLOCK_INIT;

/* The small pool directory is a linked list of directory entries. The first
 * pointer in the entry links to the next directory entry. All further pointers
 * in each entry points to a page used by the small pool.
 */
static void **os216_small_pool_directory = NULL;

/* Gets the size of the leading bitmask. */
static unsigned os216_page_bitmask_size(void){
    return (os216_nano_page_size / OS216_SIZED_MALLOC_ALIGN) >> 3;
}

/* Bytes in a small page after the bitmap. */
static unsigned os216_usable_page_size(void){
    return os216_nano_page_size - os216_page_bitmask_size();
}

/* Allocates a sized slot into an existing page. If there are no openings,
 * then this will return NULL.
 */
static void *os216_small_page_malloc(unsigned char *page, unsigned short size){
    const unsigned map_size = os216_page_bitmask_size();
    const unsigned gap_size = OS216_SMALL_ALIGN_SIZE(size);
    const size_t offset = OS216_Nano_FindBitmapGap(page, map_size, gap_size);
    if(offset > 0){
        const size_t at = offset-1;
        const size_t slot = at * OS216_SIZED_MALLOC_ALIGN;
        OS216_Nano_MarkBitmap(page, at, gap_size);
        return (page + os216_page_bitmask_size() + slot);
    }
    else{
        return NULL;
    }
}

static void *os216_small_malloc(unsigned short size){
    const unsigned short ptrs_per_page = os216_nano_page_size / sizeof(void*);
    unsigned short i;
    /* The prev is the location to put new directory entries if we do not find
     * any open space.
     */
    void ***prev = &os216_small_pool_directory;
    
    /* The empty slot is to add a new page to an existing directory if
     * possible. This avoids adding extra directory entries unless there are
     * no empty slots.
     */
    void **empty_slot = NULL;
    {
        void **at = os216_small_pool_directory;
        while(at != NULL){
            for(i = 1; i < ptrs_per_page; i++){
                /* If this slot is empty, save it as a location we could put a
                 * new entry if it comes to that.
                 */
                if(at[i] == NULL){
                    empty_slot = at + i;
                }
                else{
                    /* This page exists. Check if it has any open space. */
                    void *const allocated =
                        os216_small_page_malloc(at[i], size);
                    if(allocated != NULL)
                        return allocated;
                }
            }
            at = *at;
        }
    }
    
    /* No entries in the small pool. Add a new entry. */
    {
        if(empty_slot == NULL){
            /* Allocate a new page for the new directory entry. */
            void **const new_entry = OS216_Nano_AllocatePhysPages(1);
            
            if(new_entry == NULL)
                OS216_Nano_Fatal("Could not allocate a new frame directory");
            
            prev[0] = new_entry;
            empty_slot = new_entry+1;
            new_entry[0] = NULL;
            for(i = 2; i < ptrs_per_page; i++){
                new_entry[i] = NULL;
            }
        }
        {
            /* We need a page to put into the directory. */
            unsigned char *const new_page = OS216_Nano_AllocatePhysPages(1);
            const unsigned bitmask_size = os216_page_bitmask_size();
            empty_slot[0] = new_page;
            if(new_page == NULL)
                OS216_Nano_Fatal("Could not allocate a new page");
            for(i = 0; i < bitmask_size; i++){
                new_page[i] = 0;
            }
            {
                /* We can mark the bitmap directly since we know it's empty
                 * and our allocation is at the very start.
                 */
                const unsigned gap_size = OS216_SMALL_ALIGN_SIZE(size);
                OS216_Nano_MarkBitmap(new_page, 0, gap_size);
            }
            return new_page + bitmask_size;
        }
    }
}

static void os216_small_free(void *ptr, size_t size){
    const unsigned page_offset =
        (((long)ptr) % os216_nano_page_size) -
        os216_page_bitmask_size();
    void *const page = ((unsigned char*)ptr) - page_offset;
    const size_t offset = page_offset / OS216_SIZED_MALLOC_ALIGN;
    OS216_Nano_UnmarkBitmap(page, offset, OS216_SMALL_ALIGN_SIZE(size));
}

void *OS216_SizedMalloc(size_t size){
    void *val = NULL;
    /* The pool is divided into 8 byte elements. Divide by 8 again for bits. */
    if(size < os216_usable_page_size()){
        OS216_LockSpinlock(&os216_small_pool_lock);
        val = os216_small_malloc(size);
        OS216_UnlockSpinlock(&os216_small_pool_lock);
    }
    else{
        val = OS216_Nano_AllocatePhysPages(OS216_PAGE_ALIGN_SIZE(size));
    }
    return val;
}

void OS216_SizedFree(void *ptr, size_t size){
    if(size < os216_usable_page_size()){
        OS216_LockSpinlock(&os216_small_pool_lock);
        os216_small_free(ptr, size);
        OS216_UnlockSpinlock(&os216_small_pool_lock);
    }
    else{
        OS216_Nano_FreePhysPages(ptr, OS216_PAGE_ALIGN_SIZE(size));
    }
}
