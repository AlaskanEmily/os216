# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

all:
	$(MAKE) -f $(TOOL).mk ROOT=`pwd` ASM=yasm ASMFLAGS="-f elf32 -m x86" PLATFORM=x86 TOOLCHAIN="i486-elf-"

clean:
	$(MAKE) -f $(TOOL).mk ROOT=`pwd` PLATFORM=x86 clean

.IGNORE: clean
.PHONY: clean all
