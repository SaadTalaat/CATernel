#include <memvals.h>

#define	RELOC(x) ((x) - KERNEL_ADDR)
.set CODE_SEG,0x8
.set DATA_SEG,0x10
#define FLAGS ((1<<0) | (1<<1))
#define CHECKSUM (-( 0x1BADB002 + FLAGS))

.text
jmp _start
# The Multiboot header
.align 4
.long 0x1BADB002
.long FLAGS
.long CHECKSUM

.global		_start
_start:
	movw	$0x1234,0x472			# soft-reboot
	lgdt	RELOC(gdt_table)		# Load the GDT Register
	movl	$DATA_SEG, %eax			# Load the Data Segment
	movw	%ax,%ds				# Copy Data Segment
	movw	%ax,%es				
	movw	%ax,%ss				
	ljmp	$CODE_SEG,$boot_setup_stack		# Do a far jump to go to protected mode

boot_setup_stack:
	xor	%ebp,%ebp			# Clear the frame pointer
	movl	$(kernel_stack_end),%esp		# Load the stack into the stack pointer
	call	work_it_out

spin:	
	jmp	spin

# Virtual Page Table
############################################################################
.data
	.global	virtpgt
	.set	virtpgt, VIRTPGT
	.global	virtpgd
	.set	virtpgd, (VIRTPGT + (VIRTPGT>>10))


# Kernel Stack
############################################################################
	.p2align	PAGELG		# Will pad allocation to 0x1000 byte
	.global		kernel_stack
kernel_stack:
	.space		KERNEL_STACK
	.global		kernel_stack_end
kernel_stack_end:

#Global Descriptor table
#############################################################################
#YOU REALLY NEED TO READ THE MEMVALS HEADER BEFORE TRYING TO UNDERSTAND THIS
	.p2align	2		# pad alloc by 4
gdt_table:
	.word gdt-gdt_end-1
	.long RELOC(gdt)
gdt:
	.long 0,0
	SEGMENT(0xffffffff,-KERNEL_ADDR, SEGACS_RW|SEGACS_X)	# code seg
	SEGMENT(0xffffffff,-KERNEL_ADDR, SEGACS_RW)	# data seg
gdt_end:

