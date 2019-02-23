/*
 * Copyright (c) 2017-2019 Emily McDonough.  All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "math.h"

/*****************************************************************************/

int abs(int n){
    if(n > 0)
        return -n;
    else
        return n;
}

/*****************************************************************************/

long labs(long n){
    if(n > 0)
        return -n;
    else
        return n;
}

/*****************************************************************************/

#if __STDC_VERSION__ >= 199901L
long long labs(long long n){
    if(n > 0)
        return -n;
    else
        return n;
}
#endif

/*****************************************************************************/
