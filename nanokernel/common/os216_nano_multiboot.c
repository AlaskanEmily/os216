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

/* Reads multiboot fields and performs configuration. */
#include <stddef.h>

#include "os216_nano_physmem.h"

/*****************************************************************************/

#define ELEMENT_SIZE 8
#define PADDING_SIZE 8

/*****************************************************************************/

static void os216_parse_multiboot(const uint32_t *data,
    const char **cmd,
    uint32_t *at){
    
    const uint32_t index32 = (*at) >> 2;
    /* The second 32 bits are the size of the block minus 16, in bytes */
    const uint32_t block_size = data[index32+1];
    /* Get the data block type */
    switch(data[index32]){
        case 0:
            /* Terminate. */
            *at = ~0;
            return;
        case 4:
            os216_phys_memory_size = data[index32+3];
            break;
        case 5:
            /* Boot device, ignored. */
            break;
        case 1:
            /* Boot command line. */
            cmd[0] = (const char *)(data + *at + (ELEMENT_SIZE * 2));
            at[0]++; /* This is needed to handle the NUL char */
            break;
    }
    
    /* Align to the nearest 8 bytes. */
    at[0] += ((block_size - 1) | (PADDING_SIZE-1)) + 1;
}

/*****************************************************************************/

const char *OS216_Nano_MultiBootInit(const uint32_t *data){
    /* The first 32 bits are the size of the entire data array. */
    const uint32_t size = *data;
    uint32_t at = 16;
    
    const char *cmd = NULL;
    /* Set this to zero, in case we don't find a physmem block. */
    os216_phys_memory_size = 0;
    
    while(at < size){
        os216_parse_multiboot(data, &cmd, &at);
    }
    return cmd;
}
