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
KERN_OBJFILES := $(OBJDIR)/kernel/Reattach.o $(OBJDIR)/kernel/work_it_out.o
CC_OPTIONS= -I include

include arch/makefile
include kernel/makefile
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
	mkdir $@	
	mkdir $(OBJDIRS)
.PHONY: clean all install
install:$(OBJDIR)/kernel/CATernel.img

clean:
	rm -rf $(OBJDIR)
