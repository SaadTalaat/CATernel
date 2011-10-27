OBJDIRS += kern/kernel
KERN_OBJFILES = $(OBJDIR)/kernel/Reattach.o $(OBJDIR)/kernel/work_it_out.o

$(OBJDIR)/kernel/work_it_out.o:kernel/work_it_out.c
	$(CC) -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<

$(OBJDIR)/kernel/Reattach.o:kernel/Reattach.S $(OBJDIR)
	$(CC) -nostdinc $(CC_OPTIONS) -c -o $@ $<

$(OBJDIR)/kernel/kernel:$(KERN_OBJFILES)
	$(LD) -o $@ $< $(OBJDIR)/kernel/work_it_out.o

$(OBJDIR)/kernel/CATernel.img:$(OBJDIR)/boot/boot $(OBJDIR)/kernel/kernel
	$(DD) if=/dev/zero of=$@~ count\=10000
	$(DD) if=$(OBJDIR)/boot/boot of=$@~ conv\=notrunc
	$(DD) if=$(OBJDIR)/kernel/kernel of=$@~ conv\=notrunc seek\=1
	mv $@~ $@
