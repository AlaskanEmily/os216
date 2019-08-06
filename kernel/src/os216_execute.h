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

#ifndef OS216_EXECUTE_H
#define OS216_EXECUTE_H
#pragma once

/*****************************************************************************/
/* Wraps using liborl to run an executable file.
 * TODO: This needs to do WAY more in the future, and probably will be mostly
 * moved the process.cpp file.
 *
 * We need to perform relocations, allocate the bss segment, and setup
 * memory protection based on section at the very least.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/

void OS216_Execute(const void *file_data, unsigned file_len);

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* OS216_EXECUTE_H */
