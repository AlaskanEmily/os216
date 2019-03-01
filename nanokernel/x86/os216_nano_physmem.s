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
    xor eax, eax
    lock xchg eax, [last_freed_index]
    mov ecx, eax
    jecxz .calculate_next_index
    ; last_freed_index is usable
    dec eax
    jmp compute_address_from_bitmask
.oom:
    mov edi, edx
    xor eax, eax
    ret
.calculate_next_index
    
    ; Find the first open index. Set stuff up for wacky x86iness
    mov ecx, filled_until
    mov edx, edi ; Save edi into edx
    lea edi, [ecx+4+ecx*4] ; filled_until+4 == memory_map
    mov ecx, [ecx]
    sub ecx, PAGE_BITMASK_32_SIZE
    jna .oom ; filled_until is maxed out or oob
    
    neg ecx
    
    ; Get all F's into eax
    ; We know that eax is zero because of the mov ecx, eax/jecxz
    ; xor eax, eax
    dec eax
    cld ; Ensure direction is forward.
    repne scasd ; Find the first non-filled byte
    
    jecxz .oom
    
    xchg edi, edx ; Restore edi, put the new EDI into edx
    
    ; Update filled_until.
    
    ; ecx = PAGE_BITMASK_32_SIZE - ecx
    neg ecx
    add ecx, PAGE_BITMASK_32_SIZE
    lock xchg [filled_until], ecx
    
    not eax
    bsf ecx, eax ; Get the bit index into ecx
    ; Get 1 into eax
    xor eax, eax
    inc eax
    ; Coincidentally, ecx is the only register we can use for a shift.
    shl eax, cl
    
    ; Fill in the entry in the memory map.
    lock or [edx-4], eax
    
    ; Set eax to the entry index.
    sub edx, memory_map+4
    lea eax, [edx+ecx]
    
compute_address_from_bitmask:
    shl eax, PAGE_SHIFT
    cmp eax, (PAGE_LO_BIT_SIZE<<PAGE_SHIFT)
    jg .hi_address
.lo_address
    add eax, PAGE_LO_START
    ret
.hi_address:
    add eax, PAGE_HI_START
    ret

; Expects the address in eax, returns the resulting byte in eax and bit in ecx.
; Doesn't touch any other registers.
compute_bitmask_from_address:
    shr eax, PAGE_SHIFT ; eax = page
    mov ecx, (PAGE_HI_START>>PAGE_SHIFT)
    ; Check if the page number is above the hi start
    cmp eax, ecx
    jl .lo_address
    sub eax, ecx ; Make eax a bit index into the hi area
    ; Add the high-area offset, adjusted so we can continue through the sub below.
    add eax, (PAGE_LO_BIT_SIZE + (PAGE_LO_START>>PAGE_SHIFT))
.lo_address:
    ; Subtract the lo start to make eax a bit index
    sub eax, (PAGE_LO_START>>PAGE_SHIFT)
.bitmask_index_set:
    mov ecx, eax
    shr eax, 5
    and ecx, 0x1F ; Mask ecx to 31
    lea eax, [memory_map+eax*4]
    ret

OS216_Nano_FreePhysPage:
    mov eax, [esp+4]
    call compute_bitmask_from_address
    lock btc [eax], ecx
    
    ; Update if we just freed a page below the filled_until
    sub eax, memory_map
    mov ecx, filled_until
    cmp eax, [ecx]
    jg .no_update
    dec eax
    mov [ecx], eax
.no_update:
    ret

OS216_Nano_MarkPhysPage:
    mov eax, [esp+4]
    call compute_bitmask_from_address
    lock bts [eax], ecx
    ret

segment .rodata
    os216_nano_page_size: dd PAGE_SIZE

segment .bss
align 4
    os216_phys_memory_size: resd 1
    ; Optimization, one-entry freelist
    last_freed_index: resd 1
    ; Optimization, all entries before this 32bit index are known to be full.
    filled_until: resd 1
    memory_map: resb PAGE_BITMASK_SIZE
