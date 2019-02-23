# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

all: libc216_$(PLATFORM).a

INCFLAGS=-I./include
OBJDIR=obj_$(PLATFORM)

OBJECTS=$(OBJDIR)/c216_stdlib.o $(OBJDIR)/c216_stdio.o $(OBJDIR)/c216_string.o $(OBJDIR)/c216_ctype.o $(OBJDIR)/c216_math.o

$(OBJDIR)/c216_stdlib.o: src/c216_stdlib.c include/stdlib.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/c216_stdlib.c -o $(OBJDIR)/c216_stdlib.o

$(OBJDIR)/c216_stdio.o: src/c216_stdio.c include/stdio.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/c216_stdio.c -o $(OBJDIR)/c216_stdio.o

$(OBJDIR)/c216_string.o: src/c216_string.c include/string.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/c216_string.c -o $(OBJDIR)/c216_string.o

$(OBJDIR)/c216_ctype.o: src/c216_ctype.c include/ctype.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/c216_ctype.c -o $(OBJDIR)/c216_ctype.o

$(OBJDIR)/c216_math.o: src/c216_math.c include/math.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/c216_math.c -o $(OBJDIR)/c216_math.o

libc216_$(PLATFORM).a: $(OBJECTS)
	$(AR) rc libc216_$(PLATFORM)_x.a $(OBJECTS)
	$(RANLIB) libc216_$(PLATFORM)_x.a
	mv libc216_$(PLATFORM)_x.a libc216_$(PLATFORM).a
