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

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 ; Manages physical memory.
 ; Memory is managed in 4 or 8 bitmaps.
 ; Note that PAE is presumed to only be 36 bit in x86
 ;
 ; The allocator metadata is placed at 0x0500-0x7C00.
 ; the kernel is at 0x00100000-0x00900000
 ; The bitmap 0 is the freelist, 0x00010000-0x00080000 and 0x00900000-0x00F00000
 ; The bitmap 1 is 0x01000000-0x40000000
 ; The bitmap 2 is 0x40010000-0x80000000
 ; The bitmap 3 is 0x80000000-0xC0000000
 ; TODO: The bitmap 4 is PAE memory (if available) 0x01_00000000-0x04_00000000
 ; TODO: The bitmap 5 is PAE memory (if available) 0x04_00000000-0x08_00000000
 ; TODO: The bitmap 6 is PAE memory (if available) 0x08_00000000-0x0C_00000000
 ; TODO: The bitmap 7 is PAE memory (if available) 0x0C_00000000-0x10_00000000

 ; Bitmap 0 is size 0x00CE, placed at 0x7E00-0x7ED0 (aligned up)
 ; Bitmap 1 is size 0x7E00, placed at 0x7ED0-0xFCD0
 ; Bitmap 2 is size 0x7FFE
 ; Bitmap 3 is size 0x8000

 ; Bitmaps 2-3 are only set up if more than 1GB of memory is found.
 ; Bitmaps 2 and 3 are found at 0x40000000 (1GB) in physical memory:
 ;  - bitmap 2 is at 0x40000000-0x40007FFE
 ;  - bitmap 3 is at 0x40008000-0x40010000
 ; Bitmaps 4-7 are only set up if PAE memory is activated.
 ; TODO: bitmaps 4-7 will have dynamic locations.
SPINLOCK_ADDR equ 0x500

 ; The freelist is at 0x600. It has the following structure:
 ; 0x600-0x700: 256 count of one-byte sizes
 ; 0x700-0xA00: 256 count of three-byte 20-byte page numbers (top 4 bits are zero)
 ; empty entries have a size of 0. The address might not be zeroed.
FREE_LIST_SIZE equ 256
FREELIST_SIZE_ADDR equ 0x600
FREELIST_ENTRY_ADDR equ 0x700

 ; 0x00900000-0x00F00000 is 0x00600000 bytes (0x0600 pages, 0xC0 bytes of bitmap)
 ; 0x00010000-0x00080000 is 0x00078200 bytes (0x0078 pages, 0x0E bytes of bitmap)
BITMAP0_ADDR equ 0x7E00
BITMAP0_SIZE equ 0x00CE

BITMAP1_ADDR equ 0x7ED0
BITMAP1_SIZE equ 0x7E00

BITMAP2_ADDR equ 0x40000000
BITMAP2_SIZE equ 0x7FFE

BITMAP3_ADDR equ 0x40008000
BITMAP3_SIZE equ 0x8000

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global os216_phys_memory_size
global os216_nano_page_size
global OS216_Nano_AllocatePhysPage
global OS216_Nano_FreePhysPage
global OS216_Nano_MarkPhysPage

 ; OS216_Nano_FindBitmapGap(bitmap, map_size, gap_size)
extern OS216_Nano_FindBitmapGap
 ; void OS216_Nano_MarkBitmap(bitmap, offset, count)
extern OS216_Nano_MarkBitmap
 ; void OS216_Nano_UnmarkBitmap(bitmap, offset, count)
extern OS216_Nano_UnmarkBitmap

PAGE_SHIFT equ 12
PAGE_SIZE equ 4096 ; (1<<PAGE_SHIFT)

BITMAP_LOCKS equ 0x00000500

segment .text
 ; The nanokernel expects that it is loaded between 0x00100000 and 0x00900000,
 ; which is 1 MB to 9 MB.

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%macro SET_BLOCK 2
    mov cx, ( %1 / 4 )
    mov edi, %2
    repne stosd
%endmacro

%macro SETUP_BITMAP 1
    SET_BLOCK ( BITMAP %+ %1 %+ _SIZE ), BITMAP %+ %1 %+ _ADDR
%endmacro

OS216_Nano_InitPhysManager:
    ; Zero the freelist.
    push edi
    xor eax, eax
    xor ecx, ecx
    
    SET_BLOCK FREE_LIST_SIZE, FREELIST_SIZE_ADDR
    
    SETUP_BITMAP 0
    SETUP_BITMAP 1
    
    ; Check if we need to mess with bitmap 2 and 3
    cmp DWORD [os216_phys_memory_size], 0x40000000
    jle .skip_big_bitmaps
    
    SETUP_BITMAP 2
    SETUP_BITMAP 3
    
.skip_big_bitmaps:
    ; Set up the spinlocks.
    SET_BLOCK 16, SPINLOCK_ADDR
    ret

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 ; Search for an entry with at least bl pages.
 ; Returns 0, or the index+1
 ; dirties ecx and eax
os216_nano_freelist_search:
    xor eax, eax
    push edi
    mov edi, FREELIST_SIZE_ADDR
.find_more:
    repe scasb
    jecxz .end
    cmp bl, [edi-4]
    jg .find_more
    xor al, 0xFF
    sub eax, ecx
    inc eax
.end:
    pop edi
    ret

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Epects the loc number in eax
%macro os216_acuire_lock 1
..@acuire_lock_ %+ %1:
    mov ecx, 1
    xchg DWORD [SPINLOCK_ADDR+( %1 *4)], ecx
    ; Loop is pretty slow as it flushes the instruction cache.
    ; This is semi-useful for implementing ths spinlock.
    loop ..@acuire_lock_ %+ %1
%endmacro

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Clobbers ecx
%macro os216_release_lock 1
    xor ecx, ecx
    xchg DWORD [SPINLOCK_ADDR+( %1 *4)], ecx
%endmacro

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OS216_Nano_AllocatePhysPages:
    ; Get the number of pages to allocate.
    push ebx
    mov ebx, [esp+4]
    
    os216_acuire_lock 0
    
    ; Search the freelist if we are looking for less than 256 pages.
    cmp ebx, 0xFF
    jg .no_freelist_entry
    
    call os216_nano_freelist_search
    dec eax
    jo .no_freelist_entry
    
    ; Subtract the size we want from the size entry.
    sub [eax+FREELIST_SIZE_ADDR], dl
    lea eax, [eax*2+eax+FREELIST_ENTRY_ADDR]
    
    jnz .no_set_entry ; We zeroed it out, don't bother with setting the address
    add [eax], ebx ; Add our new offset to this value.
.no_set_entry:
    mov eax, [eax]

    ; Free the spinlock
    xor ecx, ecx
    xchg DWORD [SPINLOCK_ADDR+0x0], ecx
    
    ; Return.
    shl eax, PAGE_SHIFT
    pop ebx
    ret

.no_freelist_entry:
    ; Search bitmap 0
    push ebx
    push DWORD BITMAP0_SIZE
    push DWORD BITMAP0_ADDR
    call OS216_Nano_FindBitmapGap
    dec eax
    jo .no_bitmap0_entry
    
    ; Mark the location.
    push ebx
    push eax
    mov ebx, eax ; Save the offset. We can lose the size now.
    push DWORD BITMAP0_ADDR
    call OS216_Nano_MarkBitmap
    
    ; Free the spinlock
    os216_release_lock 0
    
    add esp, 24
    ; Offset in ebx.
    
    ; bitmap0 is the only bitmap with a split address space.
    cmp ebx, 0x0600
    jl .low_bitmap0_addr
    mov eax, (0x00900000>>PAGE_SHIFT)-0x0600
    jmp .bitmap0_addr_return
.low_bitmap0_addr:
    mov eax, (0x00010000>>PAGE_SHIFT)
.bitmap0_addr_return
    add eax, ebx
    shl eax, PAGE_SHIFT
    pop ebx
    ret
    
.no_bitmap0_entry:
    ; Free the spinlock
    os216_release_lock 0
    add esp, 12
    
    ; Try bitmap 1
    os216_acuire_lock 1
    
    push ebx
    push DWORD BITMAP1_SIZE
    push DWORD BITMAP1_ADDR
    call OS216_Nano_FindBitmapGap
    dec eax
    jo .no_bitmap1_entry
    
    ; Finalize eax in bitmap 1
    push ebx
    push eax
    mov ebx, eax ; Save the offset. We can lose the size now.
    push DWORD BITMAP1_ADDR
    call OS216_Nano_MarkBitmap
    
    ; Free the spinlock
    os216_release_lock 1
    
    add esp, 24
    lea eax, [ebx + (0x01000000 >> PAGE_SHIFT)]
    shl eax, PAGE_SHIFT
    pop ebx
    ret
    
.no_bitmap1_entry:
    os216_release_lock 1
    add esp, 12
    
    ; TODO: try bitmaps 
    xor eax, eax
    pop ebx
    ret

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OS216_Nano_MarkPhysPage:
    mov edx, OS216_Nano_MarkBitmap
    jmp os216_bitmap_change

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OS216_Nano_FreePhysPage:
    mov edx, OS216_Nano_UnmarkBitmap
    ; FALLTHROUGH
os216_bitmap_change:
    ; We can handle count upfront. We can't use the stack as-is.
    mov eax, [esp+8]
    push eax

    mov eax, [esp+4]
    cmp eax, 0x00F00000
    jl .bitmap0
    mov ecx, 0x40010000
    cmp eax, ecx
    jl .bitmap3
    ; bitmap 2
    sub eax, ecx
    mov ecx, BITMAP2_ADDR
    jmp .apply_bitmap_change
    
.bitmap3:
    sub eax, 0x80000000
    mov ecx, BITMAP3_ADDR
    jmp .apply_bitmap_change
    
.bitmap1:
    sub eax, 0x01000000
    mov ecx, BITMAP1_ADDR
    jmp .apply_bitmap_change

.bitmap0:
    mov ecx, BITMAP0_ADDR
    cmp eax, 0x00080000
    jge .bitmap0_hi
    sub eax, 0x00010000
    jmp .apply_bitmap_change
    
.bitmap0_hi:
    sub eax, 0x00900000
    xor cl, 0x0E
    ; FALLTHROUGH

.apply_bitmap_change:
    shr eax, PAGE_SHIFT
    push eax
    push ecx
    call edx
    add esp, 12
    ret

segment .rodata
    os216_nano_page_size: dd PAGE_SIZE
segment .bss
    os216_phys_memory_size: resd 1