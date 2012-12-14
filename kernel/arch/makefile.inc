OBJDIRS += $(OBJDIR)/kernel/arch\
		$(OBJDIR)/kernel/arch/$(KARCH)\
		$(OBJDIR)/kernel/arch/$(KARCH)/bios\
		$(OBJDIR)/kernel/arch/$(KARCH)/mm\
		$(OBJDIR)/kernel/arch/$(KARCH)/mp


ARCH_SOURCES = \
	kernel/arch/$(KARCH)/mm/page.c \
	kernel/arch/$(KARCH)/init_intr.S\
	kernel/arch/$(KARCH)/interrupt.c\
	kernel/arch/$(KARCH)/processor.c\
	kernel/arch/$(KARCH)/bios/bios.c\
	kernel/arch/$(KARCH)/mp/delay.c\
	kernel/arch/$(KARCH)/mp/ap.S\
	kernel/arch/$(KARCH)/mp/apic.c\
	kernel/arch/$(KARCH)/mp/smp.c\
	kernel/arch/$(KARCH)/mm/init_mem.c
	
	
