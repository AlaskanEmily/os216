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

#include "os216_malloc.h"

#include "os216_sized_malloc.h"
#include "os216_nano_fatal.h"

#include <string.h>

static unsigned os216_offset_size_len(unsigned offset){
    return (offset > 4) ? 4 : offset;
}

void *malloc(size_t size){
    size_t offset = 1;
    /* Calculate alignment requirement.
     * TODO: This should be machine-dependant.
     */
    while(size > offset && offset < 16)
        offset <<= 1;
    {
        const unsigned size_len = os216_offset_size_len(offset);
        unsigned data_size = size + offset;
        unsigned char *const block = OS216_SizedMalloc(data_size);
        unsigned i;
        
        block[offset-1] = offset;
        
        data_size--;
        for(i = 4; i > size_len; i--)
            data_size <<= 8;
        
        for(i = 0; i < size_len; i++){
            const unsigned char byte = (data_size >> 24) & 0xFF;
            block[i] = byte;
        }
        
        return block + offset;
    }
}

void *calloc(size_t i, size_t n){
    const size_t size = i*n;
    void *const val = malloc(size);
    memset(val, 0, size);
    return val;
}

void free(void *ptr){
    unsigned char *const n = ptr;
    unsigned char *const data = (n-1);
    unsigned char offset = *data;
    const unsigned size_len = os216_offset_size_len(offset);
    unsigned size, i;
    const unsigned char *const base_ptr = data - offset;
    for(i = size = 0; i < size_len; i++){
        size <<= 8;
        size += base_ptr[i];
    }
    size++;
    
    OS216_SizedFree((void*)base_ptr, size);
}
