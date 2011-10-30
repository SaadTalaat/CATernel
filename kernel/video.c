#include <x86.h>
#include <types.h>
#include <video.h>
#include <memvals.h>

uint16_t
cga_get_pos(void){
	uint16_t pos;
	outb(0x0F,CGA_INDEX1);
	pos = inb(CGA_DATA1);
	outb(0x0E,CGA_INDEX1);
	pos |= (inb(CGA_DATA1)<<8);
	return pos;
}
void
cga_set_pos(uint16_t pos){
	outb(0x0F,CGA_INDEX1);
	outb(((pos)&0xff),CGA_DATA1);
	outb(0x0E,CGA_INDEX1);
	outb((((pos)>>8) & 0xff),CGA_DATA1);
}

void 
cga_init(void){
	uint16_t *start;
	start = (uint16_t *)(KERNEL_ADDR+CGA_BUFF_OFF);
	char_buff = (uint16_t *) start;
	cursor_position = cga_get_pos();
	while(*start != 0){
		start++;
	} 
}
void
cga_putc(int c){
	c |= 0x2f00;
	char_buff[cursor_position] = c;
	cursor_position+=1;
	cga_set_pos(cursor_position);
}

cga_putstr(char *c){
	while(*c != '\0'){
		cga_putc((int )*c);
		c++;
	}

}
