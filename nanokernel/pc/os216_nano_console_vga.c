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

#include "os216_nano_console.h"

/* Implements the console using VGA. */

/*****************************************************************************/

static uint16_t *const vga = (uint16_t*)0xB8000;

/*****************************************************************************/

#define OS216_VGA_PINK (12)
#define OS216_VGA_BLUE (9)
#define OS216_VGA_CYAN (3)
#define OS216_VGA_RED (4)
#define OS216_VGA_GRAY (8)
#define OS216_VGA_BLACK (0)

/*****************************************************************************/

#define OS216_VGA_COLOR(FORE, BACK) \
    ((OS216_VGA_ ## FORE) | ((OS216_VGA_ ## BACK) << 4))

/*****************************************************************************/

static unsigned char vga_colors[] = {
    OS216_VGA_COLOR(PINK, GRAY), /* OS216_CONSOLE_NORMAL, pink on gray */
    OS216_VGA_COLOR(BLUE, GRAY), /* OS216_CONSOLE_INFO, blue on gray */
    OS216_VGA_COLOR(CYAN, GRAY), /* OS216_CONSOLE_WARNING, cyan on gray*/
    OS216_VGA_COLOR(RED, GRAY), /* OS216_CONSOLE_ERROR, red on gray */
    OS216_VGA_COLOR(BLACK, RED), /* OS216_CONSOLE_FATAL, black on red */
};

/*****************************************************************************/

const uint32_t os216_nano_console_width = 80, os216_nano_console_height = 40;

/*****************************************************************************/

#define OS216_VGA_PUT_CHAR(C, VC, X, Y) \
    vga[(X) + ((Y)*80)] = ((uint16_t)(C)) | (((uint16_t)(VC)) << 8)\

/*****************************************************************************/

void OS216_Nano_ConsolePutChar(const char c,
    const uint32_t color,
    const uint32_t x,
    const uint32_t y){
    
    OS216_VGA_PUT_CHAR(c, vga_colors[color], x, y);
}

/*****************************************************************************/

void OS216_Nano_ConsoleDuplicateChar(const char c,
    const uint32_t n,
    const uint32_t color,
    const uint32_t x,
    const uint32_t y){
    
    const uint16_t vc = vga_colors[color];
    uint32_t i;
    for(i = 0; i < n; i++){
        OS216_VGA_PUT_CHAR(c, vc, x+i, y);
    }
}

/*****************************************************************************/

uint32_t OS216_Nano_ConsolePutString(const char *const str,
    const uint32_t color,
    const uint32_t x,
    const uint32_t y){
    
    const uint16_t vc = vga_colors[color];
    uint32_t i;
    for(i = 0; str[i] != '\0'; i++){
        OS216_VGA_PUT_CHAR(str[i], vc, x+i, y);
    }
    return i;
}

/*****************************************************************************/

void OS216_Nano_ConsolePutStringN(const char *const str,
    const uint32_t len,
    const uint32_t color,
    const uint32_t x,
    const uint32_t y){
    
    const uint16_t vc = vga_colors[color];
    uint32_t i;
    for(i = 0; i < len; i++){
        OS216_VGA_PUT_CHAR(str[i], vc, x+i, y);
    }
}
