/*
 * Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "stdio.h"

/*****************************************************************************/

#define OS216_HEX_UPPER 1
#define OS216_HEX_LOWER 2
#define OS216_DEC 4
#define OS216_FLOAT 8
#define OS216_STRING 8

/*****************************************************************************/

enum os216_enum_xprint_arg{
    os216_e_hex_upper,
    os216_e_hex_lower,
    os216_e_dec,
    os216_e_oct,
    os216_e_string,
    os216_e_char
};

/*****************************************************************************/

static int xprint(char *out_buffer,
    size_t max,
    enum os216_enum_xprint_arg arg_type,
    va_list args){
    
    const char base_start = (arg_type == os216_e_hex_upper) ? 'A' : 'a';
    const int base =
        (arg_type == os216_e_dec) ? 10 :
        (arg_type == os216_e_oct) ?  8 : 16;
    
    if(max == 0)
        return 0;
    
    switch(arg_type){
        case os216_e_char:
            out_buffer[0] = va_arg(args, int);
            return 1;
        case os216_e_hex_upper: /* FALLTHROUGH */
        case os216_e_hex_lower: /* FALLTHROUGH */
        case os216_e_oct: /* FALLTHROUGH */
        case os216_e_dec:
            {
                unsigned i = 0;
                int n = va_arg(args, int);
                char buffer[32];
                if(n == 0){
                    buffer[sizeof(buffer) - ++i] = '0';
                }
                else{
                    const unsigned negate = n < 0;
                    if(negate)
                        n = -n;
                    while(n && i < sizeof(buffer)){
                        const int digit = n % base;
                        n /= base;
                        buffer[sizeof(buffer) - ++i] =
                            digit + ((digit > 9) ? base_start : '0');
                    }
                    
                    if(negate)
                        buffer[sizeof(buffer) - ++i] = '-';
                }
                
                {
                    const char *const str = buffer + (sizeof(buffer) - i);
                    unsigned at;
                    for(at = 0; at != i && at < max; at++)
                        out_buffer[at] = str[at];
                    return at;
                }
            }
        case os216_e_string:
            {
                const char *const arg = va_arg(args, char*);
                unsigned at;
                for(at = 0; at < max && arg[at] != 0; at++)
                    out_buffer[at] = arg[at];
                return at;
            }
    }
    return 0;
}

/*****************************************************************************/

int sprintf(char *buffer, const char *format, ...){
    int x;
    va_list args;
    va_start(args, format);
    x = vsnprintf(buffer, ~(size_t)0, format, args);
    va_end(args);
    return x;
}

/*****************************************************************************/

int vsprintf(char *buffer, const char *format, va_list args){
    return vsnprintf(buffer, ~(size_t)0, format, args);
}

/*****************************************************************************/

int snprintf(char *buffer, size_t n, const char *format, ...){
    int x;
    va_list args;
    va_start(args, format);
    x = vsnprintf(buffer, n, format, args);
    va_end(args);
    return x;
}

/*****************************************************************************/

int vsnprintf(char *buffer, size_t n, const char *format, va_list args){
    unsigned in_i = 0, out_i = 0, last_was_percent = 0;
    
    while(out_i+1 < n){
        const char c = format[in_i++];
        
        if(c == 0){
            break;
        }
        else if(last_was_percent){
            switch(c){
#define PRINT_TYPE(CHAR, TYPE)\
    case CHAR: \
    out_i += xprint(buffer + out_i, n - (1 + out_i), os216_e_##TYPE, args);\
    break
                PRINT_TYPE('d', dec);
                PRINT_TYPE('i', dec);
                PRINT_TYPE('o', oct);
                PRINT_TYPE('x', hex_lower);
                PRINT_TYPE('X', hex_upper);
/*                PRINT_TYPE('f', float); */
                PRINT_TYPE('s', string);
                PRINT_TYPE('c', char);
#undef PRINT_TYPE
            }
            last_was_percent = 0;
        }
        else if(c == '%'){
            last_was_percent = 1;
        }
        else {
            buffer[out_i++] = c;
        }
    }
    
    buffer[out_i] = 0;
    return out_i;
}
