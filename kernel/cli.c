#include <types.h>
#include <arch/x86/x86.h>
#include <cli.h>
#include <kbc.h>
#include <video.h>
#include <stdio.h>
void
console_init(void){
	cga_init();
	cons.wpos=0;
	cons.rpos=0;
	printk("Color Graphic Device Driver...\n");
	printk("initializing Console Layout...\n");
}
void
console_interrupt(int (*intr)(void)){
	int c;
	while ((c = (*intr)()) != -1){
		if(c==0)
			continue;
		cons.buf[cons.wpos++] = c;
		if(cons.wpos == MAXBUFSIZE)
			cons.wpos=0;
	}

}
void
console_clear(void){
	cga_clear();
}
int
console_getc(void){
	int c;
	kbc_interrupt();
	if (cons.rpos != cons.wpos){
		c = cons.buf[cons.rpos++];
		if(cons.rpos == MAXBUFSIZE)
			cons.rpos=0;
		return c;
	}
	return 0;
}

void 
console_putc(int c){
	cga_putc(c);
}
void
putchr(int c){
	console_putc(c);
}
int
getchar(void){
	int ch;
	while((ch=console_getc()) == 0);
	return ch;

}
