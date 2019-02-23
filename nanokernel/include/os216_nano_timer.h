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

#ifndef OS216_NANO_TIMER_H
#define OS216_NANO_TIMER_H
#pragma once

/* Interface for the nanokernel's timer subsystem. */

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Gets the number of available timer systems on this platform. */
extern const uint32_t os216_nano_num_timers;
#define OS216_Nano_NumTimers() (0+os216_nano_num_timers)


/*****************************************************************************/
/* Gets the string name of a timer system.
 *
 * This will at least include the following:
 *  x86:
 *      rtc
 *      pit
 *      apic
 * amd64:
 *      apic
 * dreamcast:
 *      dc
 *
 * If an invalid timer number is given, this will return NULL.
 * Note that using the APIC for either timer or interrupts requires the APIC to
 * be used for both.
 */
const char *OS216_Nano_TimerName(uint32_t timer_num);

/*****************************************************************************/

extern uint32_t os216_nano_timer_choice;
/* Gets the current timer chosen. */
#define OS216_Nano_GetTimer() (0+os216_nano_timer_choice)
/* Sets the current timer chosen. */
#define OS216_Nano_SetTimer(I) (os216_nano_timer_choice = (I))

/*****************************************************************************/

extern void (*os216_nano_timer_callback)(void*);
/* Gets the current timer callback. */
#define OS216_Nano_GetTimerCallback() (os216_nano_timer_callback)
/* Sets the current timer callback. */
#define OS216_Nano_SetTimerCallback(C) (os216_nano_timer_callback = (C))

/*****************************************************************************/

extern void *os216_nano_timer_arg;
/* Gets the current argument for the timer callback. */
#define OS216_Nano_GetTimerArg() ((void*)(((char*)os216_nano_timer_arg)+0))
/* Sets the current argument for the timer callback. */
#define OS216_Nano_SetTimerArg(A) (os216_nano_timer_arg = (A))

/*****************************************************************************/

extern uint32_t os216_nano_timer_hertz;
/* Gets the timer speed in hertz. */
#define OS216_Nano_GetTimerHertz() (os216_nano_timer_hertz+0)
/* Sets the timer speed in hertz. */
#define OS216_Nano_SetTimerHertz(H) (os216_nano_timer_hertz = (H))

/*****************************************************************************/
/* Sets up the timer system.
 * On most platforms, interrupts must be set up before the timer will actually
 * start delivering ticks.
 * It is recommended to setup the timer before setting up interrupts.
 */
void OS216_Nano_InitTimer(void);

/*****************************************************************************/
/* Performs the callback that is performed during a clock tick. */
void OS216_Nano_TimerCallback(void);

#ifdef __cplusplus
} // extern "C"
#endif

/*****************************************************************************/

#endif /* OS216_NANO_TIMER_H */
