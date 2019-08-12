# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

all: lib$(PLATFORM).a

OBJDIR=obj_$(PLATFORM)

# x86 timer drivers
X86TIMERS=$(OBJDIR)/os216_nano_timer.o $(OBJDIR)/os216_nano_timer_pit.o $(OBJDIR)/os216_nano_timer_rtc.o

$(OBJDIR)/os216_nano_timer_pit.o: x86/os216_nano_timer_pit.s x86/os216_nano_timer.inc
	$(ASM) $(ASMFLAGS) x86/os216_nano_timer_pit.s -o $(OBJDIR)/os216_nano_timer_pit.o

$(OBJDIR)/os216_nano_timer_rtc.o: x86/os216_nano_timer_rtc.s x86/os216_nano_timer.inc
	$(ASM) $(ASMFLAGS) x86/os216_nano_timer_rtc.s -o $(OBJDIR)/os216_nano_timer_rtc.o

$(OBJDIR)/os216_nano_timer.o: x86/os216_nano_timer.s x86/os216_nano_timer_rtc.inc x86/os216_nano_timer_pit.inc x86/os216_nano_apic.inc
	$(ASM) $(ASMFLAGS) x86/os216_nano_timer.s -o $(OBJDIR)/os216_nano_timer.o

# x86 interrupt drivers
X86INTERRUPTS=$(OBJDIR)/os216_nano_interrupt.o $(OBJDIR)/os216_nano_interrupt_pic.o $(OBJDIR)/os216_nano_apic.o

$(OBJDIR)/os216_nano_interrupt_pic.o: x86/os216_nano_interrupt_pic.s x86/os216_nano_interrupt.inc
	$(ASM) $(ASMFLAGS) -I ./include x86/os216_nano_interrupt_pic.s -o $(OBJDIR)/os216_nano_interrupt_pic.o

$(OBJDIR)/os216_nano_apic.o: x86/os216_nano_apic.s x86/os216_nano_interrupt.inc
	$(ASM) $(ASMFLAGS) -I ./include x86/os216_nano_apic.s -o $(OBJDIR)/os216_nano_apic.o

$(OBJDIR)/os216_nano_interrupt.o: x86/os216_nano_interrupt.s x86/os216_nano_interrupt_pic.inc x86/os216_nano_apic.inc
	$(ASM) $(ASMFLAGS) x86/os216_nano_interrupt.s -o $(OBJDIR)/os216_nano_interrupt.o

# x86 memory management
X86MEMORY=$(OBJDIR)/os216_nano_physmem.o $(OBJDIR)/os216_nano_virtmem.o

$(OBJDIR)/os216_nano_physmem.o: x86/os216_nano_physmem.s
	$(ASM) $(ASMFLAGS) x86/os216_nano_physmem.s -o $(OBJDIR)/os216_nano_physmem.o

$(OBJDIR)/os216_nano_virtmem.o: x86/os216_nano_virtmem.s
	$(ASM) $(ASMFLAGS) x86/os216_nano_virtmem.s -o $(OBJDIR)/os216_nano_virtmem.o

# Other x86 objects
$(OBJDIR)/os216_nano_init.o: x86/os216_nano_init.s
	$(ASM) $(ASMFLAGS) x86/os216_nano_init.s -o $(OBJDIR)/os216_nano_init.o

$(OBJDIR)/os216_nano_syscall.o: x86/os216_nano_syscall.s
	$(ASM) $(ASMFLAGS) x86/os216_nano_syscall.s -o $(OBJDIR)/os216_nano_syscall.o

# All x86 objects
X86OBJECTS=$(OBJDIR)/os216_nano_init.o $(OBJDIR)/os216_nano_syscall.o $(X86TIMERS) $(X86INTERRUPTS) $(X86MEMORY)

# PC objects, shared between amd64 and x86
PCOBJECTS=$(OBJDIR)/os216_nano_console_vga.o $(OBJDIR)/os216_nano_paging.o

$(OBJDIR)/os216_nano_console_vga.o: pc/os216_nano_console_vga.c include/os216_nano_console.h
	$(CC) $(CFLAGS) -I./include -c pc/os216_nano_console_vga.c -o $(OBJDIR)/os216_nano_console_vga.o

$(OBJDIR)/os216_nano_paging.o: pc/os216_nano_paging.c
	$(CC) $(CFLAGS) -I./include -c pc/os216_nano_paging.c -o $(OBJDIR)/os216_nano_paging.o

# Common objects
COMMONOBJECTS=$(OBJDIR)/os216_nano_multiboot.o $(OBJDIR)/os216_nano_fatal.o $(OBJDIR)/os216_nano_bitmap.o

$(OBJDIR)/os216_nano_multiboot.o: common/os216_nano_multiboot.c include/os216_nano_physmem.h
	$(CC) $(CFLAGS) -I./include -c common/os216_nano_multiboot.c -o $(OBJDIR)/os216_nano_multiboot.o

$(OBJDIR)/os216_nano_fatal.o: common/os216_nano_fatal.c include/os216_nano_fatal.h include/os216_nano_console.h
	$(CC) $(CFLAGS) -I./include -c common/os216_nano_fatal.c -o $(OBJDIR)/os216_nano_fatal.o

$(OBJDIR)/os216_nano_bitmap.o: common/os216_nano_bitmap.c include/os216_nano_bitmap.h
	$(CC) $(CFLAGS) -I./include -c common/os216_nano_bitmap.c -o $(OBJDIR)/os216_nano_bitmap.o

libx86.a: remove_tmp_a $(X86OBJECTS) $(PCOBJECTS) $(COMMONOBJECTS)
	$(AR) rc libx86_x.a $(X86OBJECTS) $(PCOBJECTS) $(COMMONOBJECTS)
	$(RANLIB) libx86_x.a
	mv libx86_x.a libx86.a

remove_tmp_a:
	rm libx86_x.a

clean: remove_tmp_a
	rm -rf $(OBJDIR)

.SILENT: remove_tmp_a
.PHONY: remove_tmp_a clean
.IGNORE: remove_tmp_a clean
