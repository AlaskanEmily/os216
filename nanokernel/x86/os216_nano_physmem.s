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

global os216_phys_memory_size
global os216_nano_page_size
global OS216_Nano_AllocatePhysPage
global OS216_Nano_FreePhysPage
global OS216_Nano_MarkPhysPage

PAGE_SIZE equ 4096
PAGE_BITMASK_SIZE equ 97984
PAGE_BITMASK_32_SIZE equ (PAGE_BITMASK_SIZE/4)

PAGE_LOW_BIT_SIZE equ 192
PAGE_LOW_BIT_32_SIZE equ (PAGE_LOW_BIT_SIZE/4)
PAGE_LOW_BYTE_SIZE equ (PAGE_LOW_BIT_SIZE*8)

PAGE_LOW_START equ 0x00900000
PAGE_HI_START equ 0x01000000

segment .text
; The nanokernel expects that it is loaded between 0x00100000 and 0x00900000,
; which is 1 MB to 9 MB. 

; There are 2048 pages for the kernel (TODO: use huge pages?)
; 6291456 bytes after the kernel, which is 1536 pages.
; 3204448256 bytes after ISA and before ACPI, which is 782336 pages.
; For available free memory pages, there are 783872 bits
; That totals 97984 bytes to hold all the bits, 192 bytes of which are below ISA.

OS216_Nano_AllocatePhysPage:
    xor eax, eax
    jmp .find_page_begin
.find_page_iter:
    inc eax
    ; Check if we are outside the range of the bitmask.
    cmp eax, PAGE_BITMASK_32_SIZE
    jge .oom
.find_page_begin:
    mov ecx, [memory_map+eax*4]
    not ecx
    jecxz .find_page_iter
    bsf edx, ecx
    
    ; Set that this page is used.
    not ecx
    bts ecx, edx
    mov [memory_map+eax*4], ecx
    
    mov ecx, PAGE_LOW_START
    ; Check if we are in the low map
    cmp eax, PAGE_LOW_BIT_32_SIZE
    jl .low_page
    mov ecx, PAGE_HI_START
.low_page:
    ; eax is an index for ever 32 bits.
    ; shift by 5 to change it to a bit offset
    shl eax, 5
    ; add edx for the index within the 32 bits eax points at
    add eax, edx
    ; shift by 12 for the page size.
    shl eax, 12
    ; Add the base address.
    add eax, ecx
    ret
.oom:
    xor eax, eax
    ret

; Expects the address in eax, returns the resulting byte in eax and bit in ecx.
; Doesn't touch any other registers.
phys_to_bit:
    mov ecx, PAGE_LOW_START
    cmp eax, PAGE_HI_START
    jl .low_page
    mov ecx, PAGE_HI_START-(PAGE_SIZE*PAGE_LOW_BYTE_SIZE)
.low_page:
    sub eax, ecx
    shr eax, 15
    shr ecx, 12
    and ecx, 0xFF
    ret

OS216_Nano_FreePhysPage:
    mov eax, [esp+4]
    call phys_to_bit
    btc [memory_map+eax], ecx
    ret

OS216_Nano_MarkPhysPage:
    mov eax, [esp+4]
    call phys_to_bit
    bts [memory_map+eax], ecx
    ret

segment .rodata
    os216_nano_page_size: dd PAGE_SIZE

segment .bss
    os216_phys_memory_size: resd 1
    memory_map: resb PAGE_BITMASK_SIZE
