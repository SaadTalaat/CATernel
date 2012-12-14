//I DID NOT WRITE THIS i'm only patching it
/* $Id: //depot/blt/kernel/trampoline.S#2 $
**
** Copyright Sidney Cammeresi.  All rights reserved.
** Distributed under the terms of the OpenBLT License.
/*
 * This is the initial bootstrap code for the application processors.  When
 * we get here, we are running on the AP in 16-bit real mode with a stack
 * allocated at 0x1000 * my_cpu_num by the kernel which we are not set up
 * to use yet.  Our text is at the bottom of this stack.  We have to be
 * careful until things are fixed up.
 * Basically, we just set protected mode with a temporary GDT, and call
 * C code.  The IDT and paging are set up later since we can't reference
 * any global variable here declared outside this file.
 *
 * Memory map at this stage is
 *     0x9000  location of our stack
 *     0x9004  null descriptor
 *     0x9008
 *     0x900c  kernel text descriptor
 *     0x9010
 *     0x9014  kernel data descriptor
 *     0x9018
 *     0x901c  gdt limit << 16
 *     0x9020  gdt base
 *     0x9024  address of page directory
 */

.global trampoline
.global trampoline_end
.global flush

.code16
trampoline:
		cli                      # paranoia
		xor %ax, %ax
		mov %ax, %ds
		mov %ax, %ss

		movl $0x9000, %eax       # find the location of our stack
		mov (%eax), %ebx

		xor %eax, %eax
		mov %ax, %ss
		add $0x1000, %ebx
		mov %bx, %sp

		movl $0x18, %eax         # i[0] = limit << 16; (limit is 24 dec.)
		movl $0x10, %ecx
		shl %cl, %eax
		mov $0x901c, %ebx
		mov %eax, (%ebx)
		mov $0x9004, %eax        # i[1] = base; (base = 0x9004)
		mov %eax, 4(%ebx)
		mov $0x901e, %eax
		lgdt (%eax)

		movl $0x9024, %eax
		mov (%eax), %eax
		mov %eax, %cr3

		movl $0x80000001, %eax   # turn on paging and protected mode
		mov %eax, %cr0

		/*
		 * Do a long jump to the kernel text segment to serialise the processor.
		 * A jump to flush won't work since we are being linked to run at a
		 * different address, so we calculate the offset in the segment ourselves.
		 */
		ljmp $0x8, $(0x1000 + flush - trampoline)

.code32
flush:
		mov $0x10, %ax
		mov %ax, %ds
		mov %ax, %es
		mov %ax, %fs
		mov %ax, %gs
		mov %ax, %ss
		mov $0x66, %ax
		mov $0x1000, %dx
		mov %eax, (%edx)
		cld                      # supposedly good for gcc > 2
		#movl $0xf0100000,%eax #to kernel
		jmp trampoline_end

trampoline_end:
	jmp trampoline_end

