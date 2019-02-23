/* Any copyright of this file is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

ENTRY(OS216_Nano_Main)

SECTIONS{
    /* Start at 1 MB */
    . = 1M;
    
    /* Multiboot header goes first. */
    .mutliboot BLOCK(4K) : ALIGN(4K){
        *(.multiboot)
    }
    
    /* The kernel libc goes into pages which will be shared with ring 0 and
     * ring 2. This is because the ring 2 process bootstrap must be able to
     * call the libc as well.
     */
    .shared_text BLOCK(4K) : ALIGN(4K){
        libc216/libc216_x86.a(.text)
    }
    
    .shared_rodata BLOCK(4K) : ALIGN(4K){
        libc216/libc216_x86.a(.rodata*)
        libc216/libc216_x86.a(.eh_frame*)
    }
    
    .shared_data BLOCK(4K) : ALIGN(4K){
        libc216/libc216_x86.a(.data)
    }
    
    .shared_bss BLOCK(4K) : ALIGN(4K){
        libc216/libc216_x86.a(COMMON)
        libc216/libc216_x86.a(.bss)
    }
    
    /* Ring 2 components.
     * This is where liborl and the exec stub goes. We don't want to parse any
     * metadata of an executable in Ring 0.
     * liborl exists to bootstrap processes. It is mapped in such a way that it
     * can only be executed from ring 2, not ring 0 or ring 3.
     * The libc must also be available in ring 2 to support liborl, but is used
     * from within the kernel and so is mapped in shared pages.
     */
    .ring2_only_text BLOCK(4K) : ALIGN(4K){
        liborl/liborl_x86.a(.text)
    }
    
    .ring2_only_rodata BLOCK(4K) : ALIGN(4K){
        liborl/liborl_x86.a(.rodata*)
        liborl/liborl_x86.a(.eh_frame*)
    }
    
    .ring2_only_data BLOCK(4K) : ALIGN(4K){
        liborl/liborl_x86.a(.data)
    }
    
    .ring2_only_bss BLOCK(4K) : ALIGN(4K){
        liborl/liborl_x86.a(COMMON)
        liborl/liborl_x86.a(.bss)
    }
    
    .text BLOCK(4K) : ALIGN(4K){
        *(.text*)
    }
    
    .rodata BLOCK(4K) : ALIGN(4K){
        *(.eh_frame*)
        *(.rodata*)
    }
    
    .data BLOCK(4K) : ALIGN(4K){
        *(.data)
    }
    
    .bss BLOCK(4K) : ALIGN(4K){
        *(COMMON)
        *(.bss)
    }
}