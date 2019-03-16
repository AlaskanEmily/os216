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

#ifndef OS216_NANO_FATAL_H
#define OS216_NANO_FATAL_H
#pragma once

/* Functions to call on your way out the door. */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Get a no-return attribute if possible. */
#ifdef _MSC_VER
#define OS216_NANO_NORETURN __declspec(noreturn)
#elif defined __GNUC__
#define OS216_NANO_NORETURN __attribute__((noreturn))
#else
#define OS216_NANO_NORETURN
#endif

/*****************************************************************************/
/* Prints a fatal message, and then does a halt-and-catch-fire */
OS216_NANO_NORETURN void OS216_Nano_Fatal(const char *text);

/*****************************************************************************/
/* Halts all execution.
 * Note that this is not present in the fatal C file.
 * It is usually defined in the init ASM file, as we want to disable
 * interrupts before busy-waiting for the end of the world.
 */
OS216_NANO_NORETURN void OS216_Nano_Halt(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* OS216_NANO_FATAL_H */
