/*  Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "string.h"

/*****************************************************************************/

size_t strlen(const char *const str){
    size_t i = 0;
    while(str[i] != '\0')
        i++;
    return i;
}

/*****************************************************************************/

size_t strnlen(const char *const str, const size_t n){
    size_t i = 0;
    while(i < n && str[i] != '\0')
        i++;
    return i;
}

/*****************************************************************************/

int strcmp(const char *str_a, const char *str_b){
    
    size_t i = 0;
    char a;
    
    /* Quick check, equivalent pointers will be equivalent strings. */
    if(str_a == str_b)
        return 0;
    
    do{
        const char b = str_b[i];
        a = str_a[i];
        
        if(a != b)
            return a - b;
        
        i++;
    }while(a != '\0');
    return 0;
}

/*****************************************************************************/

int strncmp(const char *str_a, const char *str_b, size_t n){
    
    size_t i = 0;
    char a;
    
    /* Quick check, equivalent pointers will be equivalent strings. */
    if(n == 0 && str_a == str_b)
        return 0;
    
    do{
        const char b = str_b[i];
        a = str_a[i];
        
        if(a != b)
            return a - b;
        
        
    }while(++i < n && a != '\0');
    return 0;
}

/*****************************************************************************/

int memcmp(const void *mem_a, const void *mem_b, size_t n){
    size_t i = 0;
    
    /* Quick check, equivalent pointers will be equivalent strings. */
    if(n == 0 && mem_a == mem_b)
        return 0;
    
    do{
        const char a = ((unsigned char*)mem_a)[i];
        const char b = ((unsigned char*)mem_b)[i];
        if(a != b)
            return a - b;
    }while(++i < n);
    return 0;
}

/*****************************************************************************/

void *memchr(const void *to, int ch, size_t len){
    size_t i;
    for(i = 0; i < len; i++){
        unsigned char *const bytep = ((unsigned char *)to) + i;
        if(*bytep == ch)
            return bytep;
    }
        
    return (void*)0;
}

/*****************************************************************************/

void *memset(void *to, int ch, size_t len){
    size_t i;
    for(i = 0; i < len; i++)
        ((unsigned char *)to)[i] = ch;
    return to;
}

/*****************************************************************************/

char *strcpy(char *to, const char *from){
    size_t i;
    for(i = 0; from[i] != '\0'; i++)
        to[i] = from[i];
    return to;
}

/*****************************************************************************/

char *strncpy(char *to, const char *from, size_t n){
    size_t i;
    for(i = 0; i < n && from[i] != '\0'; i++)
        to[i] = from[i];
    return to;
}

/*****************************************************************************/

char *strcat(char *to, const char *from){
    strcpy(to + strlen(to), from);
    return to;
}

/*****************************************************************************/

void *memcpy(void *to, const void *from, size_t len){
    size_t i;
    unsigned char *const to_bytes = to;
    const unsigned char *const from_bytes = from;
    for(i = 0; i < len; i++)
        to_bytes[i] = from_bytes[i];
    return to;
}

/*****************************************************************************/

void memset_pattern4(void *to, const void *pattern, size_t len){
    size_t i;
    for(i = 0; i < len; i++)
        ((unsigned char *)to)[i] = ((unsigned char *)pattern)[i & 3];
}

/*****************************************************************************/

void memset_pattern8(void *to, const void *pattern, size_t len){
    size_t i;
    for(i = 0; i < len; i++)
        ((unsigned char *)to)[i] = ((unsigned char *)pattern)[i & 7];
}

/*****************************************************************************/

void memset_pattern16(void *to, const void *pattern, size_t len){
    size_t i;
    for(i = 0; i < len; i++)
        ((unsigned char *)to)[i] = ((unsigned char *)pattern)[i & 15];
}
