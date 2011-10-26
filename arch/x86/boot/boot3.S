
.set PROT_MODE_CSEG,0x10	
.set PROT_MODE_DSEG,0x8        # data segment selector
.set CR0_PE_ON,0x1		# protected mode enable flag

.globl start					# Entry point	
start:		.code16				# This runs in real mode
		cli				# Disable interrupts
		cld				# String operations increment

		# Set up the important data segment registers (DS, ES, SS).
		xorw	%ax,%ax			# Segment number zero
		movw	%ax,%ds			# -> Data Segment
		movw	%ax,%es			# -> Extra Segment
		movw	%ax,%ss			# -> Stack Segment

		# Set up the stack pointer, growing downward from 0x7c00.
		movw	$start,%sp         	# Stack Pointer
	
# Enable A20:
#   For fascinating historical reasons (related to the fact that
#   the earliest 8086-based PCs could only address 1MB of physical memory
#   and subsequent 80286-based PCs wanted to retain maximum compatibility),
#   physical address line 20 is tied to low when the machine boots.
#   Obviously this a bit of a drag for us, especially when trying to
#   address memory above 1MB.  This code undoes this.
	
seta20.1:	inb	$0x64,%al		# Get status
		testb	$0x2,%al		# Busy?
		jnz	seta20.1		# Yes
		movb	$0xd1,%al		# Command: Write
		outb	%al,$0x64		#  output port
seta20.2:	inb	$0x64,%al		# Get status
		testb	$0x2,%al		# Busy?
		jnz	seta20.2		# Yes
		movb	$0xdf,%al		# Enable
		outb	%al,$0x60		#  A20

# Switch from real to protected mode:
#   Up until now, there's been no protection, so we've gotten along perfectly
#   well without explicitly telling the processor how to translate addresses.
#   When we switch to protected mode, this is no longer true!
#   We need at least to set up some "segments" that tell the processor it's
#   OK to run code at any address, or write to any address.
#   The 'gdt' and 'gdtdesc' tables below define these segments.
#   This code loads them into the processor.
#   We need this setup to ensure the transition to protected mode is smooth.

real_to_prot:	cli			# Don't allow interrupts: mandatory,
					# since we didn't set up an interrupt
					# descriptor table for handling them
		lgdt	gdt_table		# load GDT: mandatory in protected mode
		movl	%cr0, %eax	# Turn on protected mode
		orl	$CR0_PE_ON, %eax
		movl	%eax, %cr0

	        # CPU magic: jump to relocation, flush prefetch queue, and
		# reload %cs.  Has the effect of just jmp to the next
		# instruction, but simultaneously loads CS with
		# $PROT_MODE_CSEG.
		ljmp	$PROT_MODE_CSEG, $protcseg
	
		# we've switched to 32-bit protected mode; tell the assembler
		# to generate code for that mode
protcseg:	.code32
		movw	$PROT_MODE_DSEG, %ax
		movw	%ax, %ds
		movw	%ax, %es
		movw	%ax, %fs
		movw	%ax, %gs
		movw	%ax, %ss
	
#	;	call cmain			# finish the boot load from C.
						# cmain() should not return
spin:		jmp spin			# ..but in case it does, spin
	
		.p2align 2			# force 4 byte alignment
gdt_table:
          .word gdt-gdt_end-1
          .long gdt
          gdt: .long 0,0
                 .byte 0xff,0xff,0,0,0,0x92,0xcf,0
                 .byte 0xff,0xff,0,0,0,0x9A,0xcf,0
gdt_end:

