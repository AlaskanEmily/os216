/* Any copyright is dedicated to the Public Domain.
 *http://creativecommons.org/publicdomain/zero/1.0/
 */

#include "os216_nano_physmem.h"

/* Implements the slab on x86/AMD64
 * The slab on PC uses conventional memory, the area between the BIOS data
 */

const uint32_t os216_slab_size = 0x7FAFF;
void *const os216_slab_ptr = (void*)0x500;
