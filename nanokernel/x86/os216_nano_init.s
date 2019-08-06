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

; Multiboot data
MBALIGN   equ 1
MEMINFO   equ 2
FLAGS     equ MBALIGN | MEMINFO
MAGIC     equ 0x1BADB002
CHECKSUM  equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text

global OS216_Nano_Halt
global OS216_Nano_Main
global os216_nano_kernel_stack

extern OS216_Main
extern OS216_Nano_MultiBootInit

OS216_Nano_Main:
    cli
    
    ; Check for multiboot.
    cmp eax, 0x2BADB002
    je .os216_use_multiboot
    
    ; TODO! What other boot loaders do we expect? Syslinux? BTX?
    
.os216_use_multiboot:
    
    ; Load the multiboot data structure.
    ; The boot info struct is in ebx
    mov esp, os216_nano_kernel_stack
    push ebx
    ;call OS216_Nano_MultiBootInit
    
    ; Push the kernel command line.
    push eax
    
.os216_init:

    ; Set up the stack. This means the bootloader configuration code can 
    ; whatever, and doesn't even need to set up the stack if it isn't needed.
    mov esp, os216_nano_kernel_stack
    call OS216_Main
    
OS216_Nano_Halt:
    cli
    hlt
    jmp OS216_Nano_Halt

section .rodata
platform_name:
    db 'x86',0

section .bss

os216_nano_kernel_stack_bottom:
    resb 8192
os216_nano_kernel_stack:
