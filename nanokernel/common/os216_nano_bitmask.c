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

static const uint32_t os216_bitmasks[] = {
    0x00000000,
    0x00000001,
    0x00000003,
    0x00000007,
    0x0000000F,
    0x0000001F,
    0x0000003F,
    0x0000007F,
    0x000000FF,
    0x000001FF,
    0x000003FF,
    0x000007FF,
    0x00000FFF,
    0x00001FFF,
    0x00003FFF,
    0x00007FFF,
    0x0000FFFF,
    0x0001FFFF,
    0x0003FFFF,
    0x0007FFFF,
    0x000FFFFF,
    0x001FFFFF,
    0x003FFFFF,
    0x007FFFFF,
    0x0FFFFFFF,
    0x1FFFFFFF,
    0x3FFFFFFF,
    0x7FFFFFFF,
    0xFFFFFFFF
};

/* Returns 0 if there are not count leading zeroes.
 * DO NOT pass a count > 32.
 */
static int os216_nano_count_leading(const uint32_t bitmask,
    const size_t count){
    
    return bitmask & os216_bitmasks[count];
}

/* Returns how many leading bits (from most-significant) that are zero */
static size_t os216_nano_count_trailing(const uint32_t bitmask){
    
    const uint32_t mask = 0x80000000; /* Top bit set */
    size_t i = 0;
    while(((mask >> i) & bitmask) == 0)
        i++;
    return i;
}

/* Finds a gap of count within the mask. Returns the offset into the bitmask
 * plus one, or zero if no such gap exists.
 * DO NOT pass a count > 32.
 */
static size_t os216_nano_inner_gap(const uint32_t bitmask,
    const size_t count){
    
    const uint32_t mask = os216_bitmasks[count];
    uint32_t i = 0;
    do{
        const size_t result = bitmask & (mask << i);
        i++;
        if(result == 0){
            return i;
        }
    }while(i + count < 32);
    
    return 0;
}

/* Finds a gap of 32-bits or less. */
static size_t os216_nano_find_small_bitmask_gap(const uint32_t *bitmask,
    size_t max_size,
    size_t gap_size){
    
    size_t i = 0;
    while(max_size != 0){
        if(*bitmask != 0xFFFFFFFF){
            
            {
                /* Try for an inner gap first. */
                const size_t gap = os216_nano_inner_gap(*bitmask, gap_size);
                if(gap != 0)
                    return (i<<2)+gap;
            }
            
            if(max_size > 1){
                /* Check for a gap bordering the edge. */
                const size_t trail = os216_nano_count_trailing(*bitmask);
                if(trail != 0){
                    const size_t lead =
                        os216_nano_count_leading(bitmask[1], gap_size-trail);
                    if(lead == 0)
                        return (i<<2)+(32-trail);
                }
            }
        }
        max_size--;
        i++;
        bitmask++;
    }
    
    return 0;
}

/* Finds a gap greater than 32-bits. */
static size_t os216_nano_find_large_bitmask_gap(const uint32_t *bitmask,
    size_t max_size,
    size_t gap_size){
    
    size_t i;
    const size_t required = 1+(gap_size >> 5);
    while(max_size > required){
        if(*bitmask != 0xFFFFFFFF){
            const size_t trail = os216_nano_count_trailing(*bitmask);
            if(trail != 0){
                const size_t required_empty = (gap_size - trail) >> 5;
                const size_t lead_size = (gap_size - trail) & 0x31;
                size_t empty;
                for(empty = 0; empty < required_empty; empty++){
                    if(bitmask[empty+1] != 0){
                        break;
                    }
                }
                
                if(empty == required_empty){
                    /* Now search for the lead... */
                    const uint32_t lead_mask = bitmask[required_empty];
                    const size_t lead =
                        os216_nano_count_leading(lead_mask, lead_size);
                    if(lead == 0)
                        return (i<<2)+(32-trail);
                }
                else{
                    /* We can set things up to start searching again from
                     * this location... */
                    i += empty;
                    max_size -= empty;
                    bitmask += empty;
                }
            }
        }
        max_size--;
        i++;
        bitmask++;
    }
    return 0;
}

size_t OS216_Nano_FindBitmaskGap(const uint32_t *bitmask,
    size_t max_size,
    size_t gap_size){
    
    if(gap_size > 32){
        return os216_nano_find_large_bitmask_gap(bitmask,
            max_size >> 2,
            gap_size);
    }
    else{
        return os216_nano_find_small_bitmask_gap(bitmask,
            max_size >> 2,
            gap_size);
    }
}

/* Marks (sets) count bits at offset number of bits into the bitmask */
void OS216_Nano_MarkBitmask(uint32_t *bitmask,
    size_t offset,
    size_t count);

/* Unmarks (clears) count bits at offset number of bits into the bitmask */
void OS216_Nano_UnmarkBitmask(uint32_t *bitmask,
    size_t offset,
    size_t count);

