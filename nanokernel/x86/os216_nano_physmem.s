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
PAGE_SIZE equ 4096 ; (1<<PAGE_SHIFT)

; Number of bytes in the bitmap representing the lo area
PAGE_LO_BYTE_SIZE equ 192
; Number of 4-byte elements in the bitmap (useful for repne scas)
PAGE_LO_BYTE_32_SIZE equ (PAGE_LO_BYTE_SIZE/4)
; Number of bits in the bitmap used for the lo area
PAGE_LO_BIT_SIZE equ (PAGE_LO_BYTE_SIZE*8)

PAGE_LO_START equ 0x00900000 ; 9 MB
PAGE_HI_START equ 0x01000000 ; 16 MB

SCRATCH_BITMAP_DIFF equ 0x40
SCRATCH_PTR equ 0x00000500
BITMAP_ADDR equ SCRATCH_PTR+SCRATCH_BITMAP_DIFF
PAGE_BITMASK_SIZE equ 97984
PAGE_BITMASK_32_SIZE equ (PAGE_BITMASK_SIZE/4)

segment .text
; The nanokernel expects that it is loaded between 0x00100000 and 0x00900000,
; which is 1 MB to 9 MB. 

OS216_Nano_InitPhysManager:
    ; Move all zeros into the bitmap
    xor eax, eax
    mov edx, edi ; Save edi
    mov edi, BITMAP_ADDR
    mov ecx, PAGE_BITMASK_32_SIZE
    rep stosd
    mov edi, edx ; Restore edi
    inc eax
    mov [SCRATCH_PTR], eax ; Set the spinlock.
    ret

; There are 2048 pages for the kernel (TODO: use huge pages?)
; 6291456 bytes after the kernel, which is 1536 pages.
; 3204448256 bytes after ISA and before ACPI, which is 782336 pages.
; For available free memory pages, there are 783872 bits
; That totals 97984 bytes to hold all the bits, 192 bytes of which are below ISA.

OS216_Nano_AllocatePhysPage:
    mov ecx, PAGE_BITMASK_32_SIZE
    ; Set eax to all F's
    mov edx, edi ; Save edi
    mov edi, BITMAP_ADDR

    xor eax, eax
    cld ; Ensure we move the correct direction
    ; Lock the spinlock
.spin:
    lock xchg eax, [edi-SCRATCH_BITMAP_DIFF]
    cmp eax, 0
    je .spin
    
    xor eax, eax
    not eax
    repne scasd
    xchg edi, edx ; restore edi, put edi found into edx
    
    jecxz .oom
    
    ; Correct for the one extra increment that scasd does.
    mov eax, [edx-4]
    
    ; Get the upper bits
    sub edx, BITMAP_ADDR+4
    
    ; Find the first unset bit in eax
    mov ecx, 31
    
.find_next_bit:
    
    ; Since ecx is at 31 first, move out the left
    rcl eax, 1
    jnc .found
    
    loop .find_next_bit
    ; FALLTHROUGH
.oom:
    xor eax, eax
    jmp unlock_spinlock
.found:
    xor eax, eax
    inc eax
    shl eax, cl
    or [BITMAP_ADDR+edx], eax
    lea eax, [edx*4 + eax]
    ; FALLTHROUGH
    
unlock_spinlock:
    ; Unlock the spinlock
    lock or DWORD [SCRATCH_PTR], 1
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
    mov edx, eax
    shr eax, 5
    and edx, 0x1F ; Mask edx to 31
    lea eax, [BITMAP_ADDR+eax*4]
    ; FALLTHROUGH

    ; This is weird.
lock_spinlock:
    xor ecx, ecx
    push eax
    mov eax, SCRATCH_PTR
.spin
    lock xchg ecx, [eax]
    jecxz .spin
    pop eax
    ret

OS216_Nano_FreePhysPage:
    mov eax, [esp+4]
    call compute_bitmask_from_address
    btc [eax], edx
    jmp unlock_spinlock

OS216_Nano_MarkPhysPage:
    mov eax, [esp+4]
    call compute_bitmask_from_address
    bts [eax], edx
    jmp unlock_spinlock

segment .rodata
    os216_nano_page_size: dd PAGE_SIZE
segment .bss
    os216_phys_memory_size: resd 1