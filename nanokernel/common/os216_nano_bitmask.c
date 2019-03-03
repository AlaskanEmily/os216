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

#include "os216_nano_bitmask.h"

#include <stdint.h>

size_t OS216_Nano_FindBitmaskGap(const void *bitmap_v,
    size_t map_size,
    size_t gap_size){
    
    const unsigned char *const bitmap = bitmap_v;
    size_t i = 0;
    
    if(map_size == 0)
        return 0;
    
    if(gap_size == 0)
        return 1;
    
    while((i + gap_size) / 8 < map_size){
        size_t run = 0;
        uint32_t last = 0;
        
        do{
            const size_t byte = (i + run) / 8;
            const size_t bit = (i + run) % 8;
            last = bitmap[byte] & bit;
            
            if(++run == gap_size)
                return i + 1;
            
        }while(last == 0);
        
        i += run;
    }
    
    return 0;
}

/*****************************************************************************/
/* Marks (sets) count bits at offset number of bits into the bitmask */
void OS216_Nano_MarkBitmask(void *bitmask,
    size_t offset,
    size_t count);

/*****************************************************************************/
/* Unmarks (clears) count bits at offset number of bits into the bitmask */
void OS216_Nano_UnmarkBitmask(void *bitmask,
    size_t offset,
    size_t count);

