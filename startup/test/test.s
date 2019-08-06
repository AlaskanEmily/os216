 ; Any copyright is dedicated to the Public Domain.
 ; https://creativecommons.org/publicdomain/zero/1.0/

 ; The simplest possible executable. Calls the exit syscall.

section .text
global _start

_start:
    xor eax, eax
    ud2
