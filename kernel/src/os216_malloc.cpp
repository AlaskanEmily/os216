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

#include "os216_malloc.h"
#include "os216_nano_fatal.h"
#include "os216_nano_physmem.h"

// TODO! Actually allocate physical pages and be an allocator.

static unsigned amount = 0, top = 0;

void *malloc(size_t n){
    void *const v = (char*)os216_slab_ptr + amount;
    amount += n;
    
    if(amount > os216_slab_size){
        OS216_Nano_Fatal("Kernel out of memory");
    }
    
    top = n;
    return v;
}

void *calloc(size_t n, size_t i){
    const unsigned total = n * i;
    char *const v = (char*)malloc(total);
    for(unsigned a = 0; a < total; a++){
        v[a] = 0;
    }
    return v;
}

void free(void *p){
    // This is silly at best.
    if(p == (char*)os216_slab_ptr + amount){
        amount -= top;
        top = 0;
    }
}
