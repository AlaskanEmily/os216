# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

all: libsynchro_$(PLATFORM).a

OBJDIR=obj_$(PLATFORM)

# All x86 objects
X86OBJECTS=$(OBJDIR)/os216_synchro_spinlock.o

$(OBJDIR)/os216_synchro_spinlock.o: x86/os216_synchro_spinlock.s
	$(ASM) $(ASMFLAGS) x86/os216_synchro_spinlock.s -o $(OBJDIR)/os216_synchro_spinlock.o

# Common objects
COMMONOBJECTS=

libsynchro_x86.a: remove_tmp_a $(X86OBJECTS) $(COMMONOBJECTS)
	$(AR) rc libsynchro_x86_x.a $(X86OBJECTS) $(COMMONOBJECTS)
	$(RANLIB) libsynchro_x86_x.a
	mv libsynchro_x86_x.a libsynchro_x86.a

remove_tmp_a:
	rm libsynchro_$(PLATFORM)_x.a

clean: remove_tmp_a
	rm -rf $(OBJDIR)

.SILENT: remove_tmp_a
.PHONY: remove_tmp_a clean
.IGNORE: remove_tmp_a clean
