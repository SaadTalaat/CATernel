#include <types.h>
#include <cmos.h>
#include <video.h>
#include <cli.h>
#include <stdio.h>
#include <test.h>


void
work_it_out(void){
//	cga_init();
//	char *c = "BAH";
//	printk("The\r\naddress\nis\t%p\b",&"HELLO WORLD");
	console_init();
//	cursor_position=0;
//	cga_set_pos(cursor_position);
	console_clear();
	console_init();
	printk("CATernel is booting up...\n");
	bootup();
//	play();
}
