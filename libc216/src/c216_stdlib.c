/*  Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "stdlib.h"
#include "ctype.h"

/*****************************************************************************/

int atoi(const char *a){
    int x = 0, negate = 0;
    size_t i = 0;
    
    while(iswhitespace(a[i]))
        i++;
    
    if(a[i] == '-'){
        negate = 1;
        i++;
    }
    else if(a[i] == '+'){
        i++;
    }
    
    while(isdigit(a[i])){
        x += a[i++] - '0';
    }
    
    return (negate) ? (-i) : i;
}

/*****************************************************************************/
extern int c216_dummy_i;
int c216_dummy_i = 0;
void abort(void){
    /* Trigger a divide-by-zero. This is all we have. */
    c216_dummy_i /= c216_dummy_i;
}

/*****************************************************************************/

void qsort(void *const ptr, const size_t count, const size_t size,
    int(*compare)(const void *, const void *)){
    
    /* Gnome sort for stupid...I mean, simple implementation. */
    
    unsigned at = 0;
    
    if(count == 0 || count == 1)
        return;
    
    while(at < count){
        unsigned char *const byte_ptr = ((unsigned char *)ptr) + (at * size),
            *const previous_byte_ptr = byte_ptr - size;
        if(at == 0 || compare(byte_ptr, previous_byte_ptr) < 0){
            at++;
        }
        else{
            unsigned i;
            for(i = 0; i < size; i++){
                const unsigned char byte = byte_ptr[i];
                byte_ptr[i] = previous_byte_ptr[i];
                previous_byte_ptr[i] = byte;
            }
            at--;
        }
    }
}

/*****************************************************************************/
