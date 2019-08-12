/*
 *  Copyright (c) 2019 Emily McDonough.  All rights reserved.
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

#include "os216_nano_virtmem.h"

#include "os216_nano_physmem.h"
#include "os216_nano_fatal.h"

#include <stdint.h>

/* TODO: This is now outdated. */
/* Contains operations to run on page directories. These are intended to be
 * used from assembly, and so the definitions are put into the x86/amd64
 * directories.
 */

#define OS216_PC_WRT (1<<0) /* Page is writable */
#define OS216_PC_USR (1<<1) /* Page is accessible from rings >0 */
#define OS216_PC_EXE (1<<2) /* Page is executable (PAE only) */
#define OS216_PC_BIG (1<<3) /* Page is 4MB */

/* Shift flag values left by this much, or the flags input right by this much,
 * to get the paging type (x86, amd64 or PAE) */
#define OS216_PC_PAGING_TYPE_SHIFT 4
#define OS216_PC_GET_PAGE_TYPE(FLAGS) \
    ((FLAGS>>OS216_PC_PAGING_TYPE_SHIFT)&3)

#define OS216_PC_PAGING_X86 0
#define OS216_PC_PAGING_PAE 2
#define OS216_PC_PAGING_AMD 3 /* PAE flag + bit0 */
#define OS216_PC_PAGING_IS_PAE(FLAGS) \
    (FLAGS & (OS216_PC_PAGING_PAE<<OS216_PC_PAGING_TYPE_SHIFT))

/*****************************************************************************/

union OS216_PageTableEntry {
    void *ptr;
    /* Page entries are the size of a pointer on both x86 and amd64 */
    unsigned char bytes[sizeof(void*)];
    unsigned char flags;
};

/*****************************************************************************/

static void os216_setup_page_table_entry(union OS216_PageTableEntry *entry,
    int flags){
    
    entry->flags = ((flags & 3) << 1) | 1;
    entry->bytes[1] &= 0xF0;
}

/*****************************************************************************/
/* Ensures that there is a page directory entry for this index. */
static void os216_nano_ensure_directory_entry(uintptr_t *dir, int entry){
    if((dir[entry] & 1) == 0){
        int i;
        uintptr_t *const table = OS216_Nano_AllocatePhysPages(1);
        for(i = 0; i < 1024; i++){
            table[i] = 0;
        }

        {
            uintptr_t dir_entry = ((uintptr_t)table) & 0xFFFFF800;
            dir_entry |= 7; /* Set present, RW, and userland */
            dir[entry] = dir_entry;
        }
    }
}

/*****************************************************************************/

void OS216_Nano_AllocateVirtualMemory(struct OS216_Nano_MemoryDirectory *mem,
    unsigned num_pages,
    void *virt_at,
    int flags){
    
    uintptr_t *const dir = (uintptr_t*)mem;
    if((((uintptr_t)virt_at) & 0x03FF) != 0){
        OS216_Nano_Fatal("Misaligned virtual memory address");
    }
    else{
        /* TODO: This needs one more level for amd64 */
        uintptr_t directory_i = ((uintptr_t)virt_at) >> 22;
        uintptr_t entry_i = (((uintptr_t)virt_at) >> 12) & 0x03FF;
        unsigned i = 0;
        uintptr_t allocated =
            (uintptr_t)OS216_Nano_AllocatePhysPages(num_pages);
        uintptr_t *table;
        
        /* Make sure the starting table exists. */
        os216_nano_ensure_directory_entry(dir, directory_i);
        
        table = ((uintptr_t**)dir)[directory_i];
        allocated &= 0xFFFFF800;
        allocated |= 5;
        if(flags & OS216_NANO_VIRTMEM_WRITE)
            allocated |= 2;
        
        /* TODO: NX bit support for AMD64 and PAE, XD bit for Pentium 4? */
        
        /* Set all the page table entries, allocating new tables as needed. */
next_page:
        {
            table[entry_i] = allocated;
            allocated += 0x400;
            
            if(++i < num_pages){
                /* Advance to the next physical page we allocated */
                allocated += 1024;
                
                /* Move to the next directory entry, allocating a new table
                 * if necessary.
                 */
                if(++entry_i == 0x400){
                    entry_i = 0;
                    directory_i++;
                    os216_nano_ensure_directory_entry(dir, directory_i);
                }
                /* Grab the new table. */
                table = ((uintptr_t**)dir)[directory_i];
                goto next_page;
            }
        }
    }
}

/*****************************************************************************/

void OS216_Nano_SetupMapping(union OS216_PageTableEntry *root,
    void *virt_address,
    void *phys_address,
    int flags){
    /* Flags is defined low 16-bits of flags and high 16-bits of pagecount */
    const unsigned short num_pages = flags >> 16;
    (void)num_pages;
    switch(OS216_PC_GET_PAGE_TYPE(flags)){
        default: /* FALLTHROUGH, probably error */
        case OS216_PC_PAGING_X86:
            break;
        case OS216_PC_PAGING_PAE:
            break;
        case OS216_PC_PAGING_AMD:
            break;
    }
}

/*****************************************************************************/

void OS216_Nano_SetupPageTableEntry(union OS216_PageTableEntry *entry,
    void *addr,
    int flags){
    entry->ptr = addr;
    os216_setup_page_table_entry(entry, flags);
    if(flags & OS216_PC_BIG) /* Set the HUGE page bit */
        entry->flags |= 0x80;
}
