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

 ; Procedures to use the PIC for interrupts

%include 'os216_nano_fatal.inc'
%include 'os216_nano_syscall.inc'
%include 'os216_nano_timer.inc'

NUM_INTERRUPTS equ 0x30

global OS216_Nano_InitPIC

struc os216_idt_entry
    idt_entry_offset1 resw 1
    idt_entry_selector resw 1
    idt_entry_zero resb 1
    idt_entry_type resb 1
    idt_entry_offset2 resw 1
endstruc

%macro os216_create_idt_entry 1
    istruc os216_idt_entry
        at idt_entry_offset1, dw 0
        at idt_entry_selector, dw 0x08 ; Executable, kernel only
        at idt_entry_zero, db 0
        at idt_entry_type, db %1
        at idt_entry_offset2, dw 0
    iend
%endmacro

segment .text

OS216_Nano_InitPIC:

    mov cl, 16
    
%define interrupt_descriptor_offset(n) (interrupt_descriptor_data + (n * os216_idt_entry_size ))
    
    ; Dirties eax, expects cl to be 16.
%macro set_idt_entry 2
    mov eax, %2
    mov [interrupt_descriptor_offset( %1 ) + idt_entry_offset1], ax
    ror eax, cl
    mov [interrupt_descriptor_offset( %1 ) + idt_entry_offset2], ax
%endmacro
    
    ; Set up the interrupt descriptors
    set_idt_entry 0x00, divide_by_zero
    set_idt_entry 0x01, unknown
    set_idt_entry 0x02, nmi
    set_idt_entry 0x03, break
    set_idt_entry 0x04, overflow
    set_idt_entry 0x05, bounds
    set_idt_entry 0x06, invalid_instruction_or_syscall
    set_idt_entry 0x07, device_not_ready
    set_idt_entry 0x08, double_fault
    set_idt_entry 0x09, coprocessor
    set_idt_entry 0x0A, bad_tss
    set_idt_entry 0x0B, bad_segment
    set_idt_entry 0x0C, stack_fault
    set_idt_entry 0x0D, gp
    set_idt_entry 0x0E, bad_page
    set_idt_entry 0x0F, floating_point
    set_idt_entry 0x10, misalignment
    set_idt_entry 0x11, machine_check
    set_idt_entry 0x12, sse
    set_idt_entry 0x13, virtualization
    
    ; Copy from 0x14 to 0x1F
%assign i 0x14
%rep (NUM_INTERRUPTS - i)
    set_idt_entry i, unknown
%assign i i+1
%endrep
    
    ; This is a HUGE HACK for the PIT right now.
    set_idt_entry 0x20, timer_interrupt
    
    ; Send init sequence to the PICs
    mov al, 0x11
    out 0x20, al
    out 0xA0, al
    
    times 8 nop
    
    ; Set the interrupt vector offsets
    mov al, 0x20
    out 0x21, al
    mov al, 0x28
    out 0xA1, al
    
    times 8 nop
    
    ; Set the identity of PIC2 on PIC1
    mov al, 4
    out 0x21, al
    mov al, 2
    out 0xA1, al
    
    times 8 nop
    
    ; Set the mode (8086)
    mov al, 1
    out 0x21, al
    out 0xA1, al

    times 8 nop
    
    ; Unmask all interrupts
    mov al, 0
    out 0x21, al
    out 0xA1, al
    
    times 8 nop
    
    lidt [os216_idt_ptr]
    
    ret

%define int_name(n) n %+ _name
%define int_sym(n) n %+ _int

%macro fatal_interrupt 1
%1:
    push int_name( %1 )
    call OS216_Nano_Fatal
%endmacro

    fatal_interrupt divide_by_zero
    fatal_interrupt nmi
    fatal_interrupt break
    fatal_interrupt overflow
    fatal_interrupt bounds
    fatal_interrupt invalid_instruction
    fatal_interrupt device_not_ready
    fatal_interrupt double_fault
    fatal_interrupt coprocessor
    fatal_interrupt bad_tss
    fatal_interrupt bad_segment
    fatal_interrupt stack_fault
    fatal_interrupt gp
    fatal_interrupt bad_page
    fatal_interrupt floating_point
    fatal_interrupt misalignment
    fatal_interrupt machine_check
    fatal_interrupt sse
    fatal_interrupt virtualization
    fatal_interrupt unknown

invalid_instruction_or_syscall:
    push edx
    push ecx
    
    mov edx, [esp+8]
    mov cx, [edx]
    mov dx, 0x0B0F
    cmp dx, cx
    
    jne invalid_instruction
    
    ; Jump to syscall
    call OS216_Nano_Syscall
    
    pop ecx
    pop edx
    iret
    
%macro wrapper_interrupt 2
    cli
    
    push eax
    push ecx
    
    ; Push all caller-save registers for our calling convention
    pushfd
    push edx
    
    call OS216_Nano_TimerCallback
    
    pop edx
    pop ecx
    pop eax
    popfd
    
    ; End-of-interrupt
    mov al, 0x20
    out 0x20, al
    
    sti
    iret

%endmacro

timer_interrupt:
    wrapper_interrupt OS216_Nano_TimerCallback, null_timer_callback_string

segment .rodata

NAME_TABLE_PAD_SHIFT equ 5
NAME_TABLE_PAD equ (1<<NAME_TABLE_PAD_SHIFT)

null_timer_callback_string:
    db 'NULL Callback for Timer',0

divide_by_zero_name:
    db 'Divide By Zero',0

nmi_name:
    db 'NMI',0

break_name:
    db 'Break',0

overflow_name:
    db 'Overflow',0

bounds_name:
    db 'Bounds',0

invalid_instruction_name:
    db 'Invalid Instruction',0 ; AKA syscall, we must verify that we are at ud2

device_not_ready_name:
    db 'Device Not Ready',0

double_fault_name:
    db 'Double Fault',0

coprocessor_name:
    db 'Coprocessor',0

bad_tss_name:
    db 'TSS Error',0

bad_segment_name:
    db 'Segment Error',0

stack_fault_name:
    db 'Stack Fault',0

gp_name:
    db 'Protection Fault',0

bad_page_name:
    db 'Page Fault',0

floating_point_name:
    db 'Floating Point',0

misalignment_name:
    db 'Misalignment',0

machine_check_name:
    db 'Machine Check',0

sse_name:
    db 'SSE Error',0

virtualization_name:
    db 'Virtualization',0

unknown_name:
    db 'Unknown Interrupt',0

segment .data

os216_idt_ptr:
    dw os216_idt_entry_size * NUM_INTERRUPTS
    dd interrupt_descriptor_data

interrupt_descriptor_data:
%assign i 0
%rep NUM_INTERRUPTS
    os216_create_idt_entry 0x8E
%assign i i+1
%endrep
