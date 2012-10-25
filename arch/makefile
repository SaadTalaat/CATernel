$(OBJDIR)/boot/boot:$(OBJS) |$(KERN_OBJFILES)
	@$(LD) -N -e start -Ttext 0x7c00 -o $(OBJDIRS2)/boot $^
	@$(OBJCOPY) -S  --remove-section .eh_frame -O binary $(OBJDIRS2)/boot
	@echo [*] Creating boot sector.
	@python arch/x86/boot/sign.py

$(OBJDIR)/boot/boot.o:arch/x86/boot/boot.S |$(OBJDIR)
	@echo [*] Compiling $@.
	@$(CC) -fno-dwarf2-cfi-asm -nostdinc $(CC_OPTIONS) -c -o $@ $<

$(OBJDIR)/boot/main.o:arch/x86/boot/main.c |$(OBJDIR)
	@echo [*] Compiling $@.
	@$(CC) -fno-dwarf2-cfi-asm -nostdinc $(CC_OPTIONS) -Os -c -o $@ $<
#echo $(CC) $< -> $@
