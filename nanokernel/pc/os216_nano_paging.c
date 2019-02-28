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

/* Contains operations to run on page directories. These are intended to be
 * used from assembly, and so the definitions are put into the x86/amd64
 * directories.
 */

#define OS216_PC_WRT (1<<0)
#define OS216_PC_USR (1<<1)
#define OS216_PC_EXE (1<<2)
#define OS216_PC_BIG (1<<3)

/*****************************************************************************/

union OS216_PageTableEntry {
    void *ptr;
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

void OS216_Nano_SetupPageTableEntry(union OS216_PageTableEntry *entry,
    void *addr,
    int flags){
    entry->ptr = addr;
    os216_setup_page_table_entry(entry, flags);
    if(flags & OS216_PC_BIG)
        entry->flags |= 0x80;
}
