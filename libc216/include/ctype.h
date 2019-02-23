/* Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LIBC216_CTYPE_H
#define LIBC216_CTYPE_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/

int tolower(int c);

/*****************************************************************************/

int toupper(int c);

/*****************************************************************************/

int iswhitespace(int c);

/*****************************************************************************/

int isdigit(int c);

/*****************************************************************************/

int isalpha(int c);

/*****************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LIBC216_CTYPE_H */