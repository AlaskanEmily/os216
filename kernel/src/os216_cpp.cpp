//  Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
// 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
// 
// - Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
// 
// - Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
// 
// - Products derived from this software may not be called "os216", nor may
//     "216" appear in their name, without prior written permission of
//     the copyright holders.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#include "os216_cpp.hpp"

#include "os216_malloc.h"
#include "os216_nano_fatal.h"

///////////////////////////////////////////////////////////////////////////////

static OS216_NANO_NORETURN void os216_virtual_call(){
    OS216_Nano_Fatal("Pure virtual C++ call");
}

///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUC__
extern "C" void __cxa_pure_virtual() {
    os216_virtual_call();
}
#endif

///////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
int __cdecl _purecall(){
    os216_virtual_call();
}
#endif

///////////////////////////////////////////////////////////////////////////////

void *operator new(size_t size){
    return malloc(size);
}

///////////////////////////////////////////////////////////////////////////////

void *operator new[](size_t size){
    return malloc(size);
}

///////////////////////////////////////////////////////////////////////////////

void operator delete(void *p){
    free(p);
}

///////////////////////////////////////////////////////////////////////////////

void operator delete[](void *p){
    free(p);
}
