/* Any copyright of this file is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

ENTRY(OS216_Nano_Main)

SECTIONS{
    
    . = 0x3980;
    
    .bootinfo {
        *(.bootinfo)
    }
    
    .construction BLOCK(4K) : ALIGN(4K) {
        *(.init)
        crtbegin.o(.text)
        
        *(.fini)
        crtend.o(.text)
    }
    
    .text BLOCK(4K) : ALIGN(4K){
        *(.text)
    }
    
    .rodata BLOCK(4K) : ALIGN(4K){
        *(.rodata)
    }
    
    .data BLOCK(4K) : ALIGN(4K){
        *(.data)
    }
    
    .bss BLOCK(4K) : ALIGN(4K){
        *(COMMON)
        *(.bss)
    }
}