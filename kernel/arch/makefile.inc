OBJDIRS += $(OBJDIR)/kernel/arch\
		$(OBJDIR)/kernel/arch/$(KARCH)\
		$(OBJDIR)/kernel/arch/$(KARCH)/mm\

ARCH_SOURCES = \
	kernel/arch/$(KARCH)/mm/page.c \
	kernel/arch/$(KARCH)/mm/init_mem.c