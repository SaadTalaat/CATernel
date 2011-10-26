OBJDIRS += kern/kernel
$(OBJDIR)/kernel/kernel:$(KERN_OBJS)

$(OBJDIR)/kernel/Reattach.o:kernel/Reattach.S $(OBJDIR)
	$(CC) -nostdinc $(CC_OPTIONS) -c -o $@ $<

