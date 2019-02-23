/* Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LIBC216_STDIO_H
#define LIBC216_STDIO_H
#pragma once

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/

enum {
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

/*****************************************************************************/

int sprintf(char *buffer, const char *format, ...);

/*****************************************************************************/

int vsprintf(char *buffer, const char *format, va_list args);

/*****************************************************************************/

int snprintf(char *buffer, size_t n, const char *format, ...);

/*****************************************************************************/

int vsnprintf(char *buffer, size_t n, const char *format, va_list args);

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LIBC216_STDIO_H */
