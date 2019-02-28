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

PAGE_SHIFT equ 12
PAGE_SIZE equ 4096
PAGE_BITMASK_SIZE equ 97984
PAGE_BITMASK_32_SIZE equ (PAGE_BITMASK_SIZE/4)

PAGE_LO_BYTE_SIZE equ 192
PAGE_LO_BYTE_32_SIZE equ (PAGE_LO_BYTE_SIZE/4)
PAGE_LO_BIT_SIZE equ (PAGE_LO_BYTE_SIZE*8)

PAGE_LO_START equ 0x00900000
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
    ; Find out of we can just reuse the freed index
    push edi
    mov edi, last_freed_index
    stosd
    mov ecx, eax
    jecxz .calculate_next_index
    ; last_freed_index is usable
    xor ecx, ecx
    mov [edi-4], ecx
    dec eax
    jmp compute_address_from_bitmask

.calculate_next_index
    ; Find the first open index. Set stuff up for wacky x86iness
    mov edi, memory_map
    mov ecx, PAGE_BITMASK_32_SIZE
    
    ; Get all F's into eax
    ; We know that eax is zero because of the mov ecx, eax/jecxz
    ; xor eax, eax
    dec eax
    repne stosd ; Find the first non-filled byte
    not eax
    push ecx ; We need ecx for the shl below
    bsf ecx, eax ; Get the bit index into ecx
    ; Get 1 into eax
    xor eax, eax
    inc eax
    ; Coincidentally, ecx is the only register we can use for a shift.
    shl eax, cl
    ; Fill in the entry
    or [edi-4], eax
    mov eax, ecx ; Put the byte index into eax...
    pop ecx
    ; eax (al) is the 5 low bits of the index
    ; PAGE_BITMASK_32_SIZE-ecx is the upper bits
    neg ecx
    add ecx, PAGE_BITMASK_32_SIZE
    shl ecx, 5
    or eax, ecx
compute_address_from_bitmask:
    shl eax, PAGE_SHIFT
    cmp eax, (PAGE_LO_BIT_SIZE<<PAGE_SHIFT)
    jg .hi_address
.lo_address
    add eax, PAGE_LO_START
    pop edi
    ret
    
.hi_address:
    add eax, PAGE_HI_START
    pop edi
    ret

.last_freed_index_valid:

; Expects the address in eax, returns the resulting byte in eax and bit in ecx.
; Doesn't touch any other registers.
phys_to_bit:
    shr eax, PAGE_SHIFT
    mov ecx, (PAGE_HI_START>>PAGE_SHIFT)
    cmp eax, ecx
    jl .lo_address
    sub eax, ecx
    add eax, PAGE_LO_BIT_SIZE
    jmp .bitmask_index_set
.lo_address:
    sub eax, (PAGE_LO_START>>PAGE_SHIFT)
.bitmask_index_set:
    mov ecx, eax
    shr eax, 5
    and ecx, 0x1F ; 31
    ret

OS216_Nano_FreePhysPage:
    mov eax, [esp+4]
    call phys_to_bit
    btc [memory_map+eax*4], ecx
    ret

OS216_Nano_MarkPhysPage:
    mov eax, [esp+4]
    call phys_to_bit
    bts [memory_map+eax*4], ecx
    ret

segment .rodata
    os216_nano_page_size: dd PAGE_SIZE

segment .bss
    os216_phys_memory_size: resd 1
    last_freed_index: resd 1
    memory_map: resb PAGE_BITMASK_SIZE
