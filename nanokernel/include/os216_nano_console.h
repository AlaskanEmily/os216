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

#ifndef OS216_NANO_CONSOLE_H
#define OS216_NANO_CONSOLE_H
#pragma once

/* Interface for the nanokernel's console subsystem.
 *
 * You should not write past the end of a line in the console and expect it to
 * wrap to the next line.
 *
 * Be ready for the width and/or height to be zero, in the case of a dummy
 * console implementation.
 */

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* There is no guarantee the console supports all these color combinations. */
#define OS216_NANO_CONSOLE_NORMAL (0)
#define OS216_NANO_CONSOLE_INFO (1)
#define OS216_NANO_CONSOLE_WARNING (2)
#define OS216_NANO_CONSOLE_ERROR (3)
#define OS216_NANO_CONSOLE_FATAL (4)

/*****************************************************************************/

extern const uint32_t os216_nano_console_width, os216_nano_console_height;
#define OS216_Nano_GetConsoleWidth() (0+os216_nano_console_width)
#define OS216_Nano_GetConsoleHeight() (0+os216_nano_console_height)

/*****************************************************************************/
/* The +1 avoids warnings if the arguments are unsigned */
#define OS216_Nano_ConsoleInBounds(X, Y) \
    ( (X)+1 > 0 && (X) < os216_nano_console_width && \
      (Y)+1 > 0 && (Y) < os216_nano_console_height )

/*****************************************************************************/

void OS216_Nano_ConsolePutChar(char c, uint32_t color, uint32_t x, uint32_t y);

/*****************************************************************************/
/* Places n copies of c starting at X, Y. This performs no bounds checking. */
void OS216_Nano_ConsoleDuplicateChar(char c,
    uint32_t n,
    uint32_t color,
    uint32_t x,
    uint32_t y);

/*****************************************************************************/

#define OS216_Nano_ConsoleFillLine(CH, CL, Y) \
    OS216_Nano_ConsoleDuplicateChar( \
        (CH), os216_nano_console_width, (CL), 0, (Y))

/*****************************************************************************/
/* Places a string at a specified X, Y. This performs no bounds checking.
 * Returns the string's length.
 */
uint32_t OS216_Nano_ConsolePutString(const char *str,
    uint32_t color,
    uint32_t x,
    uint32_t y);

/*****************************************************************************/
/* Places N chars at a specified X, Y. This performs no bounds checking. */
void OS216_Nano_ConsolePutStringN(const char *str,
    uint32_t len,
    uint32_t color,
    uint32_t x,
    uint32_t y);

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/*****************************************************************************/

#endif /* OS216_NANO_CONSOLE_H */
