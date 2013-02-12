OBJDIR = kern
OBJ = boot.o main.o
CC = gcc
LD = ld
DD = dd
OBJCOPY = objcopy
OBJDIRS := kern/boot
OBJDIRS2 := kern/boot
KERNDIR := kern/kernel
OBJS := $(OBJDIR)/boot/boot.o $(OBJDIR)/boot/main.o
CC_OPTIONS = -I include -Wformat -Werror -Wunreachable-code -fno-stack-protector 
KERN_CFILES = $(wildcard kernel/*.c)
KARCH = x86
#KERN_OBJFILES = $($(KERN_CFILES):.c=.o)

include kernel/makefile
include arch/makefile
#$(OBJDIR)/boot/boot:$(OBJS)
#	$(LD) -N -e start -Ttext 0x7c00 -o $(OBJDIRS)/boot $^
#	$(OBJCOPY) -S -O binary $(OBJDIRS)/boot
#	python arch/x86/boot/sign.py

#$(OBJDIR)/boot/boot.o:arch/x86/boot/boot.S $(OBJDIR)
#	$(CC) -nostdinc $(CC_OPTIONS) -c -o $@ $<
#echo $(CC) $< -> $@
#$(OBJDIR)/boot/main.o:arch/x86/boot/main.c $(OBJDIR)
#	$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<
#echo $(CC) $< -> $@
all: $(OBJDIRS2)
always:
	@:
$(OBJDIR):
	@echo [*] Setting up directory tree.
	@mkdir $@	
	@mkdir -p $(OBJDIRS)

.PHONY: clean all image iso
image:$(OBJDIR)/kernel/CATernel.img
iso:image
	@echo [*] Building ISO image at $(OBJDIR)/iso..
	@mkdir -p $(OBJDIR)/iso/boot/grub
	#@mkdir $(OBJDIR)/iso/initrd
	@cp arch/$(KARCH)/grub/grub.cfg $(OBJDIR)/iso/boot/grub/
	@cp $(KERNDIR)/kernel $(OBJDIR)/iso/boot/kern.bin
	@cp tools/initrd.img $(OBJDIR)/iso/
	@grub-mkrescue -o $(OBJDIR)/CATernel.iso $(OBJDIR)/iso
	@echo [*] ISO Image at $(OBJDIR)/CATernel.iso
clean:
	@echo [*] Generated files removed.
	@rm -rf $(OBJDIR)
