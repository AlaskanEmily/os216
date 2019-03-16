 ;  Copyright (c) 2019 Emily McDonough.  All rights reserved.
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

global OS216_LockSpinlock
global OS216_UnlockSpinlock

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OS216_LockSpinlock:
    mov eax, [esp+4]
.lock_loop:
    mov ecx, 1
    xchg DWORD [eax], ecx
    ; Loop is pretty slow as it flushes the instruction cache.
    ; This is semi-useful for the purposes of implementing a spinlock.
    ; We do not require SSE and so we do not have the pause instruction.
    loop .lock_loop
    ret

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global OS216_UnlockSpinlock
    mov eax, [esp+4]
    xor ecx, ecx
    xchg DWORD [eax], ecx
    ret
