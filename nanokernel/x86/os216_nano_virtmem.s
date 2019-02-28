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

global OS216_Nano_InitVirtualMemory

TSS_LEN equ 4096

struc os216_gdt_entry
    gdt_entry_limit resb 2
    gdt_entry_base  resb 3
    gdt_entry_access resb 1
    gdt_entry_limit_flags resb 1
    gdt_entry_base_end resb 1
endstruc

ALL_FLAGS equ     0x92 ; 0b10010010
FULL_LIMIT equ    0xFFFFFFFF
KERNEL_LIMIT equ  0x00900000
NO_ACPI_LIMIT equ 0xC0000000

%macro os216_gdt_entry_data 5 ; Base, Limit, Ring, Conforming, Executable
    istruc os216_gdt_entry
        at gdt_entry_limit, db (( %2>>12 ) & 0xFF ), ((( %2 >> 12 ) >> 8 ) & 0xFF )
        at gdt_entry_base, db ( %1 & 0xFF), (( %1 >> 8 ) & 0xFF), (( %1 >> 16 ) & 0xFF)
        at gdt_entry_access, db ALL_FLAGS | ( %3 << 5 ) | ( %4 << 2 )| ( %5 << 3 )
        at gdt_entry_limit_flags, db (( (%2>>12) >> 16) & 0x0F) | 0xC0
        at gdt_entry_base_end, db (( %1 >> 24) & 0xFF)
    iend
%endmacro

%define os216_gdt_exec_entry(Base, Limit, Ring) os216_gdt_entry_data Base, Limit, Ring, 0, 1
%define os216_gdt_data_entry(Base, Limit, Ring) os216_gdt_entry_data Base, Limit, Ring, 0, 0

segment .text

OS216_Nano_InitVirtualMemory:
    ; Setup the TSS entry of the GDT
    mov eax, os216_tss_start
    mov word [os216_tss_descriptor+gdt_entry_base], ax
    shr eax, 16
    mov byte [os216_tss_descriptor+gdt_entry_base+2], al
    shr eax, 8
    mov byte [os216_tss_descriptor+gdt_entry_base_end], al
    
    ; Load the GDT
    lgdt [os216_global_descriptor_table_ptr]

    ; Enable segmentation
    mov eax, cr0
    bts eax, 0
    mov cr0, eax
    
    ; Reload the segments
    jmp 0x08:.reload_segments
.reload_segments:
    mov ax,0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    mov ax, (os216_tss_descriptor - os216_global_descriptor_table)
    ltr ax
    ret

segment .data

os216_global_descriptor_table_ptr:
    dw ((os216_global_descriptor_table_end - os216_global_descriptor_table)-1)
    dd os216_global_descriptor_table

os216_global_descriptor_table:
 ;      0x00: (unusable)
    dd 0,0

 ;      0x08: ring 0, all memory, Executable
    os216_gdt_exec_entry(0, FULL_LIMIT, 0)
 
 ;      0x10: ring 0, all memory, Data
    os216_gdt_data_entry(0, FULL_LIMIT, 0)

 ;      0x18: ring 0, kernel memory only, Executable
    os216_gdt_exec_entry(0, KERNEL_LIMIT, 0)

 ;      0x20: ring 0, kernel memory only, Data
    os216_gdt_data_entry(0, KERNEL_LIMIT, 0)

 ;      0x28: ring 0, all memory until ACPI, Executable
    os216_gdt_exec_entry(0, NO_ACPI_LIMIT, 0)

 ;      0x30: ring 0, all memory until ACPI, Data
    os216_gdt_data_entry(0, NO_ACPI_LIMIT, 0)

 ;      0x38: ring 1, all memory, Executable
    os216_gdt_exec_entry(0, FULL_LIMIT, 1)

 ;      0x40: ring 1, all memory, Data
    os216_gdt_data_entry(0, FULL_LIMIT, 1)

 ;      0x48: ring 1, all memory until ACPI, Executable
    os216_gdt_exec_entry(0, NO_ACPI_LIMIT, 1)

 ;      0x50: ring 1, all memory until ACPI, Data
    os216_gdt_data_entry(0, NO_ACPI_LIMIT, 1)

 ;      0x58: ring 2, all memory, Executable
    os216_gdt_exec_entry(0, FULL_LIMIT, 2)

 ;      0x60: ring 2, all memory, Data
    os216_gdt_data_entry(0, FULL_LIMIT, 2)

 ;      0x68: ring 2, all memory until ACPI, Executable
    os216_gdt_exec_entry(0, NO_ACPI_LIMIT, 2)

 ;      0x70: ring 2, all memory until ACPI, Data
    os216_gdt_data_entry(0, NO_ACPI_LIMIT, 2)

 ;      0x78: ring 3, all memory, Executable
    os216_gdt_exec_entry(0, FULL_LIMIT, 3)

 ;      0x80: ring 3, all memory, Data
    os216_gdt_data_entry(0, FULL_LIMIT, 3)

 ;      0x88: ring 3, all memory, until ACPI, Executable
    os216_gdt_exec_entry(0, NO_ACPI_LIMIT, 3)

 ;      0x90: ring 3, all memory, until ACPI, Data
    os216_gdt_data_entry(0, NO_ACPI_LIMIT, 3)

 ;      0x98: Task segment
os216_tss_descriptor:
    ; TSS, will be fully filled out later
    istruc os216_gdt_entry
        at gdt_entry_limit, dw TSS_LEN
        at gdt_entry_base, db 0, 0, 0
        at gdt_entry_access, db 0x89
        at gdt_entry_limit_flags, db 0x40 ; 0x40 is for small segments
        at gdt_entry_base_end, db 0
    iend

os216_global_descriptor_table_end:

segment .bss

os216_tss_start:
    resb TSS_LEN
    