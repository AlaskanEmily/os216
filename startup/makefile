
OBJDIR=obj_$(PLATFORM)

STARTUP_DATA_OBJECT=$(OBJDIR)/os216_startup_data.o

# No need for ranlib with a single object.
all: $(STARTUP_DATA_OBJECT)
	$(AR) rc os216_$(PLATFORM)_startup.a $(STARTUP_DATA_OBJECT)

$(STARTUP_DATA_OBJECT): $(PLATFORM).s build_test make_objdir
	$(ASM) $(ASMFLAGS) $(PLATFORM).s -o $(STARTUP_DATA_OBJECT)

build_test: make_test_objdir
	$(MAKE) -C test PLATFORM=$(PLATFORM) ASM=$(ASM) ASMFLAGS="$(ASMFLAGS)" LD=$(LD) LDFLAGS="$(LDFLAGS)"

clean_test:
	$(MAKE) -C test PLATFORM=$(PLATFORM) clean

clean: clean_test
	rm $(OBJDIR)

make_test_objdir:
	mkdir test/obj_$(PLATFORM)

make_objdir:
	mkdir obj_$(PLATFORM)

.PHONY: build_test clean clean_test make_test_objdir make_objdir
.SILENT: clean clean_test make_test_objdir make_objdir
.IGNORE: clean clean_test make_test_objdir make_objdir
