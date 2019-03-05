/*  Copyright (c) 2019 Emily McDonough.  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 * - Products derived from this software may not be called "os216", nor may
 *     "216" appear in their name, without prior written permission of
 *     the copyright holders.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/

typedef unsigned os216_spinlock_t;

/*****************************************************************************/

#ifdef OS216_ENABLE_SMP

/*****************************************************************************/

void OS216_LockSpinlock(os216_spinlock_t*);

/*****************************************************************************/

void OS216_UnlockSpinlock(os216_spinlock_t*);

/*****************************************************************************/

#else /* OS216_ENABLE_SMP */

/* A correct use of spinlocks in a single-threaded environment is equivalent to
 * all no-ops. We can just use sizeof() to ensure a valud argument is passed.
 */

/*****************************************************************************/

#define OS216_LockSpinlock(X) ((void)sizeof((X)))

/*****************************************************************************/

#define OS216_UnlockSpinlock(X) ((void)sizeof((X)))

/*****************************************************************************/

#endif /* OS216_ENABLE_SMP */

#ifdef __cplusplus
} // extern "C"
#endif
