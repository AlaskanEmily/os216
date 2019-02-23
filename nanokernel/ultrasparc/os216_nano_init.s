 !  Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 ! 
 !  Redistribution and use in source and binary forms, with or without
 !  modification, are permitted provided that the following conditions are met:
 ! 
 ! - Redistributions of source code must retain the above copyright notice,
 !     this list of conditions and the following disclaimer.
 ! 
 ! - Redistributions in binary form must reproduce the above copyright notice,
 !     this list of conditions and the following disclaimer in the
 !     documentation and/or other materials provided with the distribution.
 ! 
 ! - Products derived from this software may not be called "os216", nor may
 !     "216" appear in their name, without prior written permission of
 !     the copyright holders.
 ! 
 ! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ! AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 ! FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 ! IN THE SOFTWARE.
 !

.global OS216_Nano_Main
.global OS216_Nano_Halt

.section .bootinfo
 ! Boot signature of the kernel.
.long 0x01030107
.long 0x01000000
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0

.section .text
.align 16
OS216_Nano_Main:
    call 1f
1:  mov %o7, %l0
    add %l3, %o0, %l3
    ld [%l3], %l5
    
    
    ! Busy loop. This is probably not enough...
OS216_Nano_Halt:
    call OS216_Nano_Halt

