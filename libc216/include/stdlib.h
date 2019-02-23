/*  Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LIBC216_STDLIB_H
#define LIBC216_STDLIB_H
#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL

#if ( defined __GNUC__ ) && ( __GNUC__ > 2 )
#define NULL __null
#else
#define NULL ((void*)0)
#endif

#endif

/*****************************************************************************/

void qsort(void *const ptr, const size_t count, const size_t size,
    int(*compare)(const void *, const void *));

/*****************************************************************************/

void abort(void);

/*****************************************************************************/

int atoi(const char *a);

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LIBC216_STDIO_H */
