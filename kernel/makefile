include kernel/arch/makefile.inc

ARCH_OBJSA = $(addsuffix .o, $(basename $(ARCH_SOURCES)))
ARCH_OBJS = $(foreach obj, $(ARCH_OBJSA), $(OBJDIR)/$(obj:%.c=%.o))

OBJDIRS += kern/kernel
KERN_CFILES = $(wildcard kernel/*.c)
#KERN_OBJFILES = $(($(OBJDIR)/$(KERN_CFILES)):%.c=%.o)
KERN_OBJFILES = $(foreach obj,$(KERN_CFILES),$(OBJDIR)/$(obj:%.c=%.o))
#$(KERN_OBJFILES):$(KERN_CFILES)
OBJDIR =kern
USER_BINFILES = $(OBJDIR)/kernel/proc/init_elf
GCC_LIB = $(shell $(CC) -print-libgcc-file-name)

#image:$(USER_BINFILES)

$(OBJDIR)/kernel/%.o: kernel/%.c | $(OBJDIR)/kernel/Reattach.o
	@echo $(CC) $@ from $<
	@$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/proc/init_elf.o:kernel/proc/init_elf.c
	@echo userspace..
	@mkdir kern/kernel/proc
	@$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/proc/init_elf:$(OBJDIR)/kernel/proc/init_elf.o
	@$(LD)  -o $@ -T kernel/proc/script.ld -nostdlib $^

$(OBJDIR)/kernel/arch/$(KARCH)/%/%.o:kernel/arch/$(KARCH)/%/%.c
	@echo $(CC) $@ from $<
	@$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/arch/$(KARCH)/%.o:kernel/arch/$(KARCH)/%.c
	
	@$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/arch/$(KARCH)/%.o:kernel/arch/$(KARCH)/%.S
	$(CC) -nostdinc $(CC_OPTIONS) -c -o $@ $<

#$(OBJDIR)/kernel/work_it_out.o:kernel/work_it_out.c
#$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

#new
#$(OBJDIR)/kernel/init_elf.o:kernel/proc/init_elf.c
#	$(CC) -nostdinc $(CC_OPTIONS) -c -o $@ $<

$(OBJDIR)/kernel/Reattach.o:kernel/Reattach.S
	$(CC) -nostdinc $(CC_OPTIONS) -c -o $@ $<

$(OBJDIR)/kernel/kernel:$(OBJDIR)/kernel/Reattach.o $(KERN_OBJFILES) $(ARCH_OBJS) 
#$(USER_BINFILES)
	$(LD)  -o $@ -T kernel/script.ld -nostdlib $(GCC_LIB) $< $(KERN_OBJFILES) $(ARCH_OBJS) 
#-b binary $(USER_BINFILES)
	nm -n $@ > kern/kernel/CATernel.sym

$(OBJDIR)/kernel/CATernel.img:$(OBJDIR)/boot/boot $(OBJDIR)/kernel/kernel
	$(DD) if=/dev/zero of=$@~ count\=10000
	$(DD) if=$(OBJDIR)/boot/boot of=$@~ conv\=notrunc
	$(DD) if=$(OBJDIR)/kernel/kernel of=$@~ conv\=notrunc seek\=1
	$(DD) if=/dev/zero of=$@~ count\=4096 conv\=notrunc seek\=118
	$(DD) if=kernel/proc/init_elf of=$@~ conv\=notrunc seek\=126
	mv $@~ $@
