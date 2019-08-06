# Any copyright is dedicated to the Public Domain.
# https://creativecommons.org/publicdomain/zero/1.0/

all: test

test: test.o
	$(LD) $(LDFLAGS) test.o -o test

test.o: test.s
	$(ASM) $(ASMFLAGS) test.s -o test.o
