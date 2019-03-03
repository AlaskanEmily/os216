# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

all: libos216_$(PLATFORM).a

NANOKERNELROOTDIR=../nanokernel
NANOKERNELDIR=$(NANOKERNELROOTDIR)/include

LIBC216ROOTDIR=../libc216
LIBC216DIR=$(LIBC216ROOTDIR)/include

INCFLAGS=-I$(NANOKERNELDIR) -I$(LIBC216DIR) -I./include
OBJDIR=obj_$(PLATFORM)

OBJECTS=$(OBJDIR)/os216_init.o $(OBJDIR)/os216_malloc.o $(OBJDIR)/os216_cpp.o

INITHEADERS=$(NANOKERNELDIR)/os216_nano_init.h  $(NANOKERNELDIR)/os216_nano_timer.h  $(NANOKERNELDIR)/os216_nano_interrupt.h  $(NANOKERNELDIR)/os216_nano_virtmem.h 
$(OBJDIR)/os216_init.o: src/os216_init.cpp $(INITHEADERS)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c src/os216_init.cpp -o $(OBJDIR)/os216_init.o

$(OBJDIR)/os216_malloc.o: src/os216_malloc.c src/os216_malloc.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/os216_malloc.c -o $(OBJDIR)/os216_malloc.o

$(OBJDIR)/os216_cpp.o: src/os216_cpp.cpp src/os216_cpp.hpp src/os216_malloc.h
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c src/os216_cpp.cpp -o $(OBJDIR)/os216_cpp.o

libos216_$(PLATFORM).a: remove_tmp_a $(OBJECTS) $(NANOKERNELROOTDIR)/$(PLATFORM)/os216_nano.ld
	$(AR) rc libos216_$(PLATFORM)_x.a $(OBJECTS)
	$(RANLIB) libos216_$(PLATFORM)_x.a
	mv libos216_$(PLATFORM)_x.a libos216_$(PLATFORM).a

remove_tmp_a:
	rm libos216_$(PLATFORM)_x.a

clean: remove_tmp_a
	rm -rf obj_$(PLATFORM)

.SILENT: remove_tmp_a
.PHONY: remove_tmp_a clean
.IGNORE: remove_tmp_a clean