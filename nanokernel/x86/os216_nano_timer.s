 ;  Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 ; 
 ;  Redistribution and use in source and binary forms, with or without
 ;  modification, are permitted provided that the following conditions are met:
 ; 
 ; - Redistributions of source code must retain the above copyright notice,
 ;     this list of conditions and the following disclaimer.
 ; 
 ; - Redistributions in binary form must reproduce the above copyright notice,
 ;     this list of conditions and the following disclaimer in the
 ;     documentation and/or other materials provided with the distribution.
 ; 
 ; - Products derived from this software may not be called "os216", nor may
 ;     "216" appear in their name, without prior written permission of
 ;     the copyright holders.
 ; 
 ; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 ; FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 ; IN THE SOFTWARE.

%include 'os216_nano_timer_rtc.inc'
%include 'os216_nano_timer_pit.inc'
%include 'os216_nano_apic.inc'
%include 'os216_nano_fatal.inc'

NUM_TIMER_CHOICES equ 3

global OS216_Nano_TimerName
global OS216_Nano_InitTimer
global OS216_Nano_TimerCallback

global os216_nano_timer_choice
global os216_nano_timer_callback
global os216_nano_timer_arg
global os216_nano_timer_hertz

segment .text

OS216_Nano_TimerName:
    mov eax, [esp+4]
    cmp eax, NUM_TIMER_CHOICES
    jl .timer_valid
    xor eax, eax
    ret
.timer_valid:
    mov eax, [timer_name_table + ecx * 4]
    ret

OS216_Nano_InitTimer:
    mov eax, [os216_nano_timer_choice]
    jmp [timer_init_table + eax * 4]

OS216_Nano_TimerCallback:
    push DWORD [os216_nano_timer_arg]
    mov ecx, [os216_nano_timer_callback]
    jecxz null_timer_callback
    call ecx
    add esp, 4
timer_nop:
    ret

null_timer_callback:
    push null_timer_error_message
    call OS216_Nano_Fatal

segment .rodata
    os216_nano_num_timers: dd NUM_TIMER_CHOICES
    timer_name_table:
        dd pit_timer_name, rtc_timer_name, os216_nano_apic_name
    timer_init_table:
        dd OS216_Nano_InitPIT, timer_nop, timer_nop
    pit_timer_name: db 'pit', 0
    rtc_timer_name: db 'rtc', 0
    null_timer_error_message: db 'NULL Timer Callback'

segment .bss
    os216_nano_timer_choice: resd 1
    os216_nano_timer_callback: resd 1
    os216_nano_timer_arg: resd 1
    os216_nano_timer_hertz: resd 1
