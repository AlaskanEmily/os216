/*
 *  Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
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

#ifndef OS216_NANO_VIRTMEM_H
#define OS216_NANO_VIRTMEM_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Inits the VM system (if required by the platform), but does not start up
 * any paging or the page directory.
 *
 *  x86:
 *      This function sets up segmentation.
 *      
 *      The nanokernel supports all rings 0-3, and also has segments to select
 *      all memory, only kernel memory, or all memory until the ACPI table.
 *      
 *      The layout of the selectors is:
 *      0x00: (unusable)
 *      0x08: ring 0, all memory, Executable
 *      0x10: ring 0, all memory, Data
 *      0x18: ring 0, kernel memory only, Executable
 *      0x20: ring 0, kernel memory only, Data
 *      0x28: ring 0, all memory until ACPI, Executable
 *      0x30: ring 0, all memory until ACPI, Data
 *      0x38: ring 1, all memory, Executable
 *      0x40: ring 1, all memory, Data
 *      0x48: ring 1, all memory until ACPI, Executable
 *      0x50: ring 1, all memory until ACPI, Data
 *      0x58: ring 2, all memory, Executable
 *      0x60: ring 2, all memory, Data
 *      0x68: ring 2, all memory until ACPI, Executable
 *      0x70: ring 2, all memory until ACPI, Data
 *      0x78: ring 3, all memory, Executable
 *      0x80: ring 3, all memory, Data
 *      0x88: ring 3, all memory, until ACPI, Executable
 *      0x90: ring 3, all memory, until ACPI, Data
 *      0x98: TSS segment
 */
void OS216_Nano_InitVirtualMemory(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* OS216_NANO_VIRTMEM_H */
