#include <types.h>
#include <cli.h>
#include <stdio.h>
#include <video.h>
#include <cmos.h>
#include <x86.h>
#include <cpuid.h>
void play(void);
void
time_print(void){
	char *Month[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char *Day[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	char* month_result;
	uint8_t month= 0x0;
	month = cmos_get_time(RTC_MONTH);
	if((month & ~0xf)){
		month = (month & 0xf)+10;
	}
        printk("Date: ");
        printk("20%x",(uint32_t) cmos_get_time(RTC_YEAR));
        printk(" %d",month);
        printk(", %x",(uint32_t) cmos_get_time(RTC_DAY_MONTH));
	printk(" %s\n",Day[cmos_get_time(RTC_DAY_WEEK)]); 

}
void
bootup(void){
	char ch,*pch;
	int i;
	time_print();
	scan_memory();
	setup_mem_x86_32();
	play();
}
void
play(){
	const char *buf;
	printk("Welcome to CatOS!\n");
	printk("For Commands type help\n");
	printk("This was developed by CATReloaded team: http://catreloaded.net\n\n");
	cga_set_attr(COLOR_RED|COLOR_GREEN);
	while(1){
		buf = readline("CatOS(@) ");
		if (buf!=NULL){
			printk("Command was %s\n",buf);			
		}

	}
}
