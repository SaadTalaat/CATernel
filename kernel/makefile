include kernel/arch/makefile.inc

ARCH_OBJSA = $(addsuffix .o, $(basename $(ARCH_SOURCES)))
ARCH_OBJS = $(foreach obj, $(ARCH_OBJSA), $(OBJDIR)/$(obj:%.c=%.o))

OBJDIRS += $(OBJDIR)/kernel \
		$(OBJDIR)/kernel/proc \
		$(OBJDIR)/kernel/drivers\
		$(OBJDIR)/kernel/synchronization\
		$(OBJDIR)/kernel/time\
		$(OBJDIR)/kernel/generic\
		$(OBJDIR)/kernel/structs\
		$(OBJDIR)/kernel/syscalls

KERN_CFILES = $(wildcard kernel/generic/*.c)
KERN_CFILES += $(wildcard kernel/proc/*.c)
KERN_CFILES += $(wildcard kernel/drivers/*.c)
KERN_CFILES += $(wildcard kernel/syscalls/*.c)
KERN_CFILES += $(wildcard kernel/synchronization/*.c)
KERN_CFILES += $(wildcard kernel/time/*.c)
KERN_CFILES += $(wildcard kernel/structs/*.c)
#KERN_OBJFILES = $(($(OBJDIR)/$(KERN_CFILES)):%.c=%.o)
KERN_OBJFILES = $(foreach obj,$(KERN_CFILES),$(OBJDIR)/$(obj:%.c=%.o))
#$(KERN_OBJFILES):$(KERN_CFILES)
OBJDIR =kern
USER_BINFILES = $(OBJDIR)/kernel/proc/init_elf
GCC_LIB = $(shell $(CC) -print-libgcc-file-name)

#image:$(USER_BINFILES)

$(OBJDIR)/kernel/%.o: kernel/%.c | $(OBJDIR)/kernel/Reattach.o
	@echo [*] Compiling $@
	@$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

#$(OBJDIR)/kernel/drivers/%.o:kernel/drivers/%.c
#	@echo [*] Drivers.
#	@mkdir kern/kernel/drivers
#	@$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/proc/init_elf.o:kernel/proc/init_elf.c
	@echoa [*]userspace..
	@mkdir kern/kernel/proc
	@$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/tmp/init_elf:$(OBJDIR)/kernel/proc/init_elf.o
	@$(LD)  -o $@ -T kernel/proc/script.ld -nostdlib $^

$(OBJDIR)/kernel/arch/$(KARCH)/%/%.o:kernel/arch/$(KARCH)/%/%.c
	@echo [*] Compiling $@
	@$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/arch/$(KARCH)/%.o:kernel/arch/$(KARCH)/%.c
	@$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/arch/$(KARCH)/%.o:kernel/arch/$(KARCH)/%.S
	@$(CC) -nostdinc $(CC_OPTIONS) -c -o $@ $<

$(OBJDIR)/kernel/Reattach.o:kernel/generic/Reattach.S
	@$(CC) -nostdinc $(CC_OPTIONS) -c -o $@ $<

$(OBJDIR)/kernel/kernel:$(OBJDIR)/kernel/Reattach.o $(KERN_OBJFILES) $(ARCH_OBJS) 
#$(USER_BINFILES)
	@echo [*] Compiling the kernel binary.
	@$(LD)  -o $@ -T kernel/script.ld -nostdlib $(GCC_LIB) $< $(KERN_OBJFILES) $(ARCH_OBJS) 
#-b binary $(USER_BINFILES)
	@nm -n $@ > kern/kernel/CATernel.sym

$(OBJDIR)/kernel/CATernel.img:$(OBJDIR)/boot/boot $(OBJDIR)/kernel/kernel
	@echo [*] Creating disk image at $@.
	@$(DD) if=/dev/zero of=$@~ count\=10000
	@$(DD) if=$(OBJDIR)/boot/boot of=$@~ conv\=notrunc
	@$(DD) if=$(OBJDIR)/kernel/kernel of=$@~ conv\=notrunc seek\=1
	@$(DD) if=kernel/tmp/init_elf of=$@~ conv\=notrunc seek\=$(shell stat -c%b kern/kernel/kernel)
	@mv $@~ $@
