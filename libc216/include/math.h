/* Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LIBC216_MATH_H
#define LIBC216_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/

int abs(int n);

/*****************************************************************************/

long labs(long n);

/*****************************************************************************/

#if __STDC_VERSION__ >= 199901L
long long labs(long long n);
#endif

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LIBC216_MATH_H */
