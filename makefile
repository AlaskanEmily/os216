# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

all:
	$(MAKE) -f x86.mk TOOL=gcc

clean:
	$(MAKE) -f x86.mk TOOL=gcc clean
