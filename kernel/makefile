OBJDIRS += kern/kernel
KERN_CFILES = kernel/work_it_out.c kernel/cmos.c
#KERN_OBJFILES = $(($(OBJDIR)/$(KERN_CFILES)):%.c=%.o)
KERN_OBJFILES = $(foreach obj,$(KERN_CFILES),$(OBJDIR)/$(obj:%.c=%.o))
#$(KERN_OBJFILES):$(KERN_CFILES)
OBJDIR =kern
$(OBJDIR)/kernel/%.o: kernel/%.c | $(OBJDIR)/kernel/Reattach.o
	@echo $(KERN_OBJFILES)
	$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

#$(OBJDIR)/kernel/work_it_out.o:kernel/work_it_out.c
#$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/Reattach.o:kernel/Reattach.S
	$(CC) -nostdinc $(CC_OPTIONS) -c -o $@ $<

$(OBJDIR)/kernel/kernel:$(OBJDIR)/kernel/Reattach.o $(KERN_OBJFILES)
	$(LD)  -T kernel/script.ld -nostdlib -o $@ $^
	nm -n $@ > kern/kernel/CATernel.sym

$(OBJDIR)/kernel/CATernel.img:$(OBJDIR)/boot/boot $(OBJDIR)/kernel/kernel
	$(DD) if=/dev/zero of=$@~ count\=10000
	$(DD) if=$(OBJDIR)/boot/boot of=$@~ conv\=notrunc
	$(DD) if=$(OBJDIR)/kernel/kernel of=$@~ conv\=notrunc seek\=1
	mv $@~ $@
