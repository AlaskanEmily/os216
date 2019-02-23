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

%include 'os216_nano_interrupt_pic.inc'
%include 'os216_nano_apic.inc'

NUM_INTERRUPT_CONTROLLER_CHOICES equ 2

global os216_nano_num_interrupt_controllers
global os216_nano_interrupt_controller_choice
global OS216_Nano_InterruptControllerName
global OS216_Nano_InitInterruptController
global OS216_Nano_EnableInterrupts
global OS216_Nano_DisableInterrupts

segment .text
OS216_Nano_InterruptControllerName:
    mov eax, [esp+4]
    shr eax, 1
    jnz .return_null
    rcl al, 3
    add eax, interrupt_controller_table
    ret
.return_null:
    xor eax, eax
    ret

OS216_Nano_InitInterruptController:
    mov eax, [os216_nano_interrupt_controller_choice]
    jmp [interrupt_controller_init_table + eax * 4]

OS216_Nano_DisableInterrupts:
    cli
    ret

OS216_Nano_EnableInterrupts:
    sti
    ret

segment .rodata
    os216_nano_num_interrupt_controllers: dd NUM_INTERRUPT_CONTROLLER_CHOICES
    interrupt_controller_table:
        dd pic_interrupt_controller_name, os216_nano_apic_name
    interrupt_controller_init_table:
        dd OS216_Nano_InitPIC, OS216_Nano_InitAPIC
    pic_interrupt_controller_name: db 'pic', 0

segment .bss
    os216_nano_interrupt_controller_choice: resd 1
