#include <memvals.h>
.text
.set CODE_SEG,0x8	#If you gave the gdt table at th end of the page a look you'll find that code segment desc is 0x8
.set DATA_SEG,0x10	#Same goes for data segment desc

	.global _start

_start:
	movw 	$1234,0x472		#soft reboot
	lgdt 	(gdt_table-KERNEL_ADDR)		#Load GDT register
	mov 	$DATA_SEG,%eax		#Load all\
	mov 	%ax,%ds			# Segment registers\
	mov 	%ax,%ss			# with the \
	mov 	%ax,%es			# data segment descriptor value
	ljmp	$CODE_SEG,$get_to_work	#Make a far jump to go to protected mode

get_to_work:
	xor %ebp,%ebp			#Clear the Frame pointer
	mov $kern_stack_end,%esp	# Load stack pointer with the kernel stack address
	call work_it_out
# Virtual Page Table
#############################################################################
.data
	#SETUP THE VIRTUAL PAGE TABLE
	.global virtpgt
	.set virtpgt,VIRTPGT	
	.global virtpgd
	.set virtpgd,(VIRTPGT + (VIRTPGT >> 10))

# Kernel Stack
#############################################################################
	.p2align PAGELG		#Will Pad the allocation to 0x1000 byte
	.global kern_stack
kern_stack:
	.space	KERNEL_STACK
	.global kern_stack_end
kern_stack_end:
# Global Descriptor table
#############################################################################
#YOU REALLY NEED TO READ THE MEMVALS HEADER BEFORE TRYING TO UNDERSTAND THIS
.p2align 2 #Pad alloc by 4
gdt_table:
        .word gdt-gdt_end-1
        .long gdt-KERNEL_ADDR 	#Map the gdt table
gdt:
        .long 0,0
	SEGMENT(0xFFFFFFFF,-KERNEL_ADDR,SEGACS_RW|SEGACS_X) 	#Make a code segment desc
	SEGMENT(0xFFFFFFFF,-KERNEL_ADDR,SEGACS_RW)		#Make a data segment desc
gdt_end:
