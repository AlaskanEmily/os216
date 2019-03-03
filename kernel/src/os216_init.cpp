//  Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
// 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
// 
// - Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
// 
// - Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
// 
// - Products derived from this software may not be called "os216", nor may
//     "216" appear in their name, without prior written permission of
//     the copyright holders.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#include "os216_nano_init.h"
#include "os216_nano_timer.h"
#include "os216_nano_interrupt.h"
#include "os216_nano_virtmem.h"
#include "os216_nano_console.h"

#include <stdio.h>

// For testing purposes
static int n = 0;
static void timer_callback(void *x){
    (void)x;
    n++;
    if((n % 100) == 0){
        char buffer[80];
        sprintf(buffer, "%i", (n / 100));
        OS216_Nano_ConsolePutString(buffer, OS216_NANO_CONSOLE_NORMAL, 1, 4);
    }
}

extern "C"
void OS216_Main(const char *maybe_kernel_cmd_line){
    OS216_Nano_SetTimerHertz(100);
    OS216_Nano_SetTimerCallback(timer_callback);
    OS216_Nano_InitTimer();
    OS216_Nano_InitInterruptController();
    OS216_Nano_InitVirtualMemory();
    
    OS216_Nano_EnableInterrupts();
    
    // Clear the screen
    for(unsigned y = 0; y < OS216_Nano_GetConsoleHeight(); y++)
        OS216_Nano_ConsoleFillLine(' ', OS216_NANO_CONSOLE_NORMAL, y);
    
    // Hello, world
    OS216_Nano_ConsolePutString("Welcome to the 216th OS",
        OS216_NANO_CONSOLE_NORMAL, 0, 0);
    while(1){}
}
