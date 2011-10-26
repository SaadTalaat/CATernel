.global start
.set Load,0x7c00
start:
	.code16
	cld
	xorw %ax,%ax
	movw %ax,%ss
	movw %ax,%ds
	movw %ax,%es

	movw $Load,%sp
	movw %si,%sp
	movw $start,%di
	movw $0x100,%cx
	rep
	movsw
	movw %di,%bp
	movb $0x8,%cl

A20.1:
	inb     $0x64,%al
        testb   $0x2,%al
        jnz     A20.1
        movb    $0xd1,%al
        outb    %al,$0x64
A20.2:
	inb 	$0x64,%al
	testb 	$0x2,%al
	jnz 	A20.2
	movb 	$0xd1,%al
	outb	%al,$0x60
switch_mode:
	lgdt 	gdt_table
	mov 	%cr0,%eax
	orl 	$1,%eax
	mov 	%eax,%cr0
	ljmp 	$0x10,$protseg

protseg:
	.code32
	movw $8,%ax
	movw %ax,%ds
	movw %ax,%ss
	movw %ax,%es
	movw %ax,%fs
	movw %ax,%gs
	call main
spin:
	jmp spin
.p2align 2
gdt_table:
	.word gdt-gdt_end-1
	.long gdt
gdt:
	.long 0,0
	.byte 0xff,0xff,0,0,0,0x92,0xCF,0
	.byte 0xff,0xff,0,0,0,0x9A,0xCF,0
gdt_end:

