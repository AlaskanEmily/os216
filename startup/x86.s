 ;  Copyright (c) 2019 Emily McDonough. All rights reserved.
 ; 
 ; This Source Code Form is subject to the terms of the Mozilla Public
 ; License, v. 2.0. If a copy of the MPL was not distributed with this
 ; file, You can obtain one at https://mozilla.org/MPL/2.0/.

 ; Silly use of an assembler to embed binary files.

section .rodata
align 4

os216_test_executable_data:
incbin "test/test"
os216_test_executable_size:
    dd (os216_test_executable_size-os216_test_executable_data)
