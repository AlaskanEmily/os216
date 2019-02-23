/* Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LIBC216_STRING_H
#define LIBC216_STRING_H
#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/

size_t strlen(const char *str);

/*****************************************************************************/

size_t strnlen(const char *str, size_t n);

/*****************************************************************************/

int strcmp(const char *str_a, const char *str_b);

/*****************************************************************************/

int strncmp(const char *str_a, const char *str_b, size_t n);

/*****************************************************************************/

int memcmp(const void *mem_a, const void *mem_b, size_t n);

/*****************************************************************************/

void *memchr(const void *to, int ch, size_t len);

/*****************************************************************************/

void *memset(void *to, int ch, size_t len);

/*****************************************************************************/

char *strcpy(char *to, const char *from);

/*****************************************************************************/

char *strncpy(char *to, const char *from, size_t n);

/*****************************************************************************/

char *strcat(char *to, const char *from);

/*****************************************************************************/

void *memcpy(void *to, const void *from, size_t len);

/*****************************************************************************/

void memset_pattern4(void *to, const void *pattern, size_t len);

/*****************************************************************************/

void memset_pattern8(void *to, const void *pattern, size_t len);

/*****************************************************************************/

void memset_pattern16(void *to, const void *pattern, size_t len);

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LIBC216_STRING_H */
