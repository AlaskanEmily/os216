/* Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LIBC216_ALLOCA_H
#define LIBC216_ALLOCA_H
#pragma once

#ifdef __GNUC__
#pragma once
#define alloca __builtin_alloca
#endif

#endif /* LIBC216_ALLOCA_H */
