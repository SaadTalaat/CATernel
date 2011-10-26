; Co-operation: CATReloaded 
; Author : Saad Talaat
; Copyrights (c) 2011 Saad Talaat - CATReloaded
; 
; Most of the code is based on the Unix boot manager
[ORG 0x7c00]
jmp start
        Load    db 0x7c00       ; Boot image start address
        Origin  db 0x600        ; Relocated address of Boot img
        Prt     db 0x1be        ; Partition Table
        sign    db 0xaa55       ; Boot img signature
	A201	db 0x64		; Keyboard controller read status port
	A202	db 0x60		; Keyboard controller data port


	global start

	start:
		[BITS 16]
		; We will initialize all segment registers to 0 and set the stack address below our entry point
		cld 	; Clear the direction flag and the data pointer gets decremented
		xor ax,ax	; Clear ax
		mov ds,ax	
		mov ss,ax
		mov es,ax
		mov sp,Load	; Load the stack pointer with out entry point.
;		mov si,sp
;		mov di,start
;		mov cx,100h
;		rep
;		movsw
;		; Set bp to point at the partition table
;		mov bp,di
;		; Clear words
;		mov cl,8h
;		rep
;		stosw
;		sub di,0xe
;		inc di
;	read_prt:
;		mov bx,Prt
;		add bx,4
;		xor dx,dx
;	read_one:
;		mov [bx-4],ch
	A20.1:
		in al,0x64
		test al,0x2 ; if input buffer full
		jnz A20.1
		mov al,0xd1 ; next byte written will be written to 0x60 
		out 0x64,al
	A20.2:
		in al,0x64
		test al,0x2
		jnz A20.2
		mov al,0xdf
		out 0x60,al ; Enable Address line A20

	switch_mode:
		lgdt [gdt_table] ; Load gdt table
		mov eax,cr0	; Load the Cache Register 0
		or al,1h	; set the protected mode flag on
		mov cr0,eax	; return the value to the cr0
		jmp protseg	; jmp to convert addressing mode to the protected mode
	protseg:
		[BITS 32]
		mov ax,8h	;Load the data segment into ax
		mov ds,ax	; Load
		mov ss,ax	; Them
		mov es,ax	; All
		mov fs,ax	; with
		mov gs,ax	; the data segment
		;call main0
	spin:
		jmp spin

	gdt_table:
		dw 0x17
		dd gdt
		gdt dd 0,0
		flatdesc        db 0xff,0xff,0,0,0,10010010b,11001111b,0
		codedesc        db 0xff,0xff,0,0,0,10011010b,11001111b,0

	gdt_end:
times 510-($-$$) db 0
db 0x55
db 0xAA
