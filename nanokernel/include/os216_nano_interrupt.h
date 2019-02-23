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

#ifndef OS216_NANO_INTERRUPT_H
#define OS216_NANO_INTERRUPT_H
#pragma once

/* Interface for the nanokernel's interrupt subsystem. */

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Gets the number of available timer systems on this platform. */
extern const uint32_t os216_nano_num_interrupt_controllers;
#define OS216_Nano_NumInterruptControllers() \
    (0+os216_nano_num_interrupt_controllers)

/*****************************************************************************/
/* Gets the string name of a timer system.
 *
 * This will at least include the following:
 *  x86:
 *      pic
 *      apic
 * amd64:
 *      apic
 * dreamcast:
 *      dc
 *
 * If an invalid timer number is given, this will return NULL
 */
const char *OS216_Nano_InterruptControllerName(uint32_t timer_num);

/*****************************************************************************/

extern uint32_t os216_nano_interrupt_controller_choice;
/* Gets the current interrupt controller chosen. */
#define OS216_Nano_GetInterruptController() \
    (0+os216_nano_interrupt_controller_choice)
/* Sets the current timer chosen. */
#define OS216_Nano_SetInterruptController(I) \
    (os216_nano_interrupt_controller_choice = (I))

/*****************************************************************************/
/* Sets up the timer system.
 * On most platforms, interrupts must be set up before the timer will actually
 * start delivering ticks.
 * It is recommended to setup the timer before setting up interrupts.
 */
void OS216_Nano_InitInterruptController(void);

/*****************************************************************************/
/* Enables interrupts. The system starts with interrupts disabled.
 */
void OS216_Nano_EnableInterrupts(void);

/*****************************************************************************/
/* Disables interrupts. The system starts with interrupts disabled.
 */
void OS216_Nano_DisableInterrupts(void);

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/*****************************************************************************/

#endif /* OS216_NANO_TIMER_H */
