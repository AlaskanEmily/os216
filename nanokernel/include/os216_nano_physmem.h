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

#ifndef OS216_NANO_PHYSMEM_H
#define OS216_NANO_PHYSMEM_H
#pragma once

/* Interface for the nanokernel's physical memory subsystem.
 * 
 * The nanokernel expects that the kernel data is loaded at the location:
 *  x86:
 *      0x00100000-0x00900000
 * 
 * The nanokernel expects the following memory locations to be usable:
 *  x86:
 *      0x00900000-0x00F00000 (7 MB between code and ISA memory)
 *      0x01000000-0xC0000000 (Memory above ISA, below ACPI, etc)
 */

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Size of a page on this architecture.
 */
extern const uint32_t os216_nano_page_size;

/*****************************************************************************/
/* This should only be set by the init routine (nano_multiboot, etc). */
extern uint32_t os216_phys_memory_size;

/*****************************************************************************/
void OS216_Nano_InitPhysManager(void);

/*****************************************************************************/
/* Claims a free page of physical memory.
 *
 * This will only return NULL on OOM.
 *
 * Memory should be freed using OS216_Nano_FreePhysPages
 */
void *OS216_Nano_AllocatePhysPages(int num);

/*****************************************************************************/
/* Marks that a physical page starting at addr is used.
 *
 * This should only be used when you know /why/ that page must be used, such as
 * for memory-mapped hardware, or for data or code loaded at hardcoded
 * locations.
 *
 * Memory marked with this can be freed again using OS216_Nano_FreePhysPage
 */
void OS216_Nano_MarkPhysPages(void *, int num);

/*****************************************************************************/
/* Frees a page of physical memory.
 *
 * Double-frees are OK.
 */
void OS216_Nano_FreePhysPages(void *, int num);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* OS216_NANO_PHYSMEM_H */
