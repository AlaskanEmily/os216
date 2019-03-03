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

#ifndef OS216_NANO_BITMASK_H
#define OS216_NANO_BITMASK_H
#pragma once

/* Operations on 32-bit aligned bitmasks. */

#include <stddef.h>

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Finds a gap of at least gap_size in the bitmask.
 * returns the bit offset into bitmask where the gap was found plus one, or
 * zero if no suitable gap was found.
 */
size_t OS216_Nano_FindBitmaskGap(const void *bitmap,
    size_t max_size,
    size_t gap_size);

/*****************************************************************************/
/* Marks (sets) count bits at offset number of bits into the bitmask */
void OS216_Nano_MarkBitmask(void *bitmap,
    size_t offset,
    size_t count);

/*****************************************************************************/
/* Unmarks (clears) count bits at offset number of bits into the bitmask */
void OS216_Nano_UnmarkBitmask(void *bitmap,
    size_t offset,
    size_t count);

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/*****************************************************************************/

#endif /* OS216_NANO_BITMASK_H */
