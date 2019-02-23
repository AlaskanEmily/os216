/*
 * Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ctype.h"

/*****************************************************************************/

int tolower(int c){
    if(c >= 'A' && c <= 'Z')
        return (c - 'A') + 'a';
    else
        return c;
}

/*****************************************************************************/

int toupper(int c){
    if(c >= 'a' && c <= 'z')
        return (c - 'a') + 'A';
    else
        return c;
}

/*****************************************************************************/

int iswhitespace(int c){
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v';
}

/*****************************************************************************/

int isdigit(int c){
    return c >= '0' && c <= '9';
}

/*****************************************************************************/

int isalpha(int c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c & 0x80);
}