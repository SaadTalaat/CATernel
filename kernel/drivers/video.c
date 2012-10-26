/**
 * @addtogroup Drivers
 * @{
 */ 
/**
 * @file video.c
 * @author Saad Talaat
 * @date Sunday 30/10/2011
 * @brief Color Graphics Adapter (CGA) driver.
 * @name CGA Driver
 * @{
 *
 * An early initialization for the console can be useful in debugging
 * and logging operations.
 * here screen is initialized and get ready for I/O operations.
 */
#include <arch/x86/x86.h>
#include <types.h>
#include <video.h>
#include <memvals.h>
#include <kbc.h>

/** @var cga_attr;
 * @brief holds the colors attribute of the written character
 */
static cga_attr;
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
cga_set_attr(uint16_t c){
	cga_attr = c;
}
void
cga_clear(void){
	int toclear = cursor_position;
	while(toclear > 0){
		char_buff[toclear--] = COLOR_WHITE | ' ';
	}
	cursor_position= toclear;
	cga_set_pos(toclear);
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
	cga_attr = 0;
}
void
cga_putc(int c){

	if(cga_attr != 0){
		c |= cga_attr;
	}
	else if(!(c & ~0xff)){
		c |= COLOR_WHITE;
	}
	switch(c & 0xff){
	case '\b':
		if(cursor_position > 0){
			cursor_position--;
			char_buff[cursor_position]= (c & ~0xff) | ' ';
			cga_set_pos(cursor_position);
		}
		break;
	case '\n':
		cursor_position+=CGA_COLS;
		cga_set_pos( cursor_position - (cursor_position % 0x50) );
	case '\r':
		cursor_position -= (cursor_position % CGA_COLS);
		break;
	case '\t':
		cga_putc(' ');
		cga_putc(' ');
		cga_putc(' ');
		cga_putc(' ');
		break;
	default:
		char_buff[cursor_position] = c;
		cursor_position+=1;
		cga_set_pos(cursor_position);

	}
	//char_buff[cursor_position] = c;
	//cursor_position+=1;
	//cga_set_pos(cursor_position);
	if(cursor_position >= CGA_SIZE){
		int i;
		memcopy( char_buff, char_buff+CGA_COLS, (CGA_SIZE-CGA_COLS)*sizeof(uint16_t) );
		for(i=CGA_SIZE-CGA_COLS; i< CGA_SIZE;i++){
			char_buff[i]= COLOR_WHITE | ' ';
		}
		cursor_position-= CGA_COLS;
	
	}
}

void
cga_putstr(char *c){
	while(*c != '\0'){
		cga_putc((int )*c);
		c++;
	}

}

/**
 *@} @}
 */
