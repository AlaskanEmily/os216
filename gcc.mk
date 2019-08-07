# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

KERNELBIN=os216_$(PLATFORM).elf

all: $(KERNELBIN)

CC=$(TOOLCHAIN)gcc
CXX=$(TOOLCHAIN)g++
LD=$(TOOLCHAIN)ld

OBJCOPY=$(TOOLCHAIN)objcopy
STRIP=$(TOOLCHAIN)strip
AR=$(TOOLCHAIN)ar
RANLIB=$(TOOLCHAIN)ranlib

INCLUDEDIRFLAG=-I
LDFLAGS=-g $(ARCHLINKFLAGS)
CCFLAGS=-Wall -Werror -pedantic -g -pipe -nostdlib -ffreestanding -fno-plt -O2 $(ARCHCCFLAGS)
CFLAGS=$(CCFLAGS) -ansi $(ARCHCFLAGS)
CXXFLAGS=$(CCFLAGS) -fno-rtti -fno-exceptions -std=c++98 $(ARCHCXXFLAGS)

nanokernel/lib$(PLATFORM).a: make_nanokernel_objdir
	$(MAKE) -C nanokernel PLATFORM=$(PLATFORM) ASM=$(ASM) ASMFLAGS="$(ASMFLAGS)" CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) RANLIB=$(RANLIB)

clean_nanokernel:
	$(MAKE) -C nanokernel PLATFORM=$(PLATFORM) clean

kernel/libos216_$(PLATFORM).a: make_kernel_objdir
	$(MAKE) -C kernel PLATFORM=$(PLATFORM) CXX=$(CXX) CXXFLAGS="$(CXXFLAGS)" CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) RANLIB=$(RANLIB)

clean_kernel:
	$(MAKE) -C kernel PLATFORM=$(PLATFORM) clean

build_liborl:
	$(MAKE) -C liborl static CC=$(CC) CFLAGS="$(CFLAGS) -I../libc216/include" AR=$(AR) RANLIB=$(RANLIB) INCLUDEFLAGS=-I../libc216/include
	cp liborl/liborl-static.a liborl/liborl_$(PLATFORM).a

liborl/liborl_$(PLATFORM).a: build_liborl

clean_liborl:
	$(MAKE) -C liborl clean

build_startup:
	$(MAKE) -C startup CC=$(CC) CFLAGS="$(CFLAGS)" ASM=$(ASM) ASMFLAGS="$(ASMFLAGS)" AR=$(AR) RANLIB=$(RANLIB) LD=$(LD) LDFLAGS="$(LDFLAGS)"

startup/os216_$(PLATFORM)_startup.a: build_startup

clean_startup:
	$(MAKE) -C startup clean

libc216/libc216_$(PLATFORM).a: make_libc216_objdir
	$(MAKE) -C libc216 PLATFORM=$(PLATFORM) CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) RANLIB=$(RANLIB)

clean_libc216:
	$(MAKE) -C libc216 PLATFORM=$(PLATFORM) clean

synchro/libsynchro_$(PLATFORM).a: make_synchro_objdir
	$(MAKE) -C synchro PLATFORM=$(PLATFORM) ASM=$(ASM) ASMFLAGS="$(ASMFLAGS)" CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) RANLIB=$(RANLIB)

clean_synchro:
	$(MAKE) -C synchro PLATFORM=$(PLATFORM) clean

KERNELLIBS=kernel/libos216_$(PLATFORM).a nanokernel/lib$(PLATFORM).a libc216/libc216_$(PLATFORM).a liborl/liborl_$(PLATFORM).a startup/os216_$(PLATFORM)_startup.a synchro/libsynchro_$(PLATFORM).a
LIBGCCCMD=$(TOOLCHAIN)gcc --print-file-name=libgcc.a
LINKERSCRIPT=nanokernel/$(PLATFORM)/os216_nano.ld

$(KERNELBIN): $(KERNELLIBS) $(LINKERSCRIPT)
	$(CXX) $(CXXFLAGS) -Wl,--whole-archive -o $(KERNELBIN) $(KERNELLIBS) `$(LIBGCCCMD)` -T $(LINKERSCRIPT)
	$(OBJCOPY) --only-keep-debug $(KERNELBIN) os216_$(PLATFORM).sym
	$(STRIP) -s $(KERNELBIN)

clean: clean_nanokernel clean_kernel clean_elf clean_synchro
	rm os216_$(PLATFORM).elf

make_nanokernel_objdir:
	mkdir nanokernel/obj_$(PLATFORM)

make_kernel_objdir:
	mkdir kernel/obj_$(PLATFORM)

make_libc216_objdir:
	mkdir libc216/obj_$(PLATFORM)

make_synchro_objdir:
	mkdir synchro/obj_$(PLATFORM)

clean_elf:
	rm $(KERNELBIN)

.IGNORE: clean clean_elf clean_nanokernel clean_kernel clean_liborl clean_startup clean_synchroclean_libc216 make_nanokernel_objdir make_kernel_objdir make_libc216_objdir make_synchro_objdir
.SILENT: make_nanokernel_objdir make_kernel_objdir make_lib216_objdir make_synchro_objdir
.PHONY: clean clean_elf clean_nanokernel clean_kernel clean_liborl clean_startup clean_synchroclean_libc216 make_nanokernel_objdir make_kernel_objdir make_libc216_objdir make_synchro_objdir build_liborl build_startup
