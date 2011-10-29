#include <types.h>
#include <x86.h>
#include <cmos.h>
uint8_t
cmos_get_seconds(void){
	uint8_t seconds;
	uint8_t update;
	/* First check that Register A is not in update mode */
	while(update == 0x80){
	outb(RTC_STATUS_A,CMOS_INDEXPORT);
	update = inb(CMOS_DATAPORT);
	}
	cli();
	outb(RTC_SECONDS,CMOS_INDEXPORT);
	seconds = inb(CMOS_DATAPORT);
	return seconds;
}

uint8_t
cmos_get_day(void){
        uint8_t seconds;   
        uint8_t update;
        /* First check that Register A is not in update mode */
        while(update == 0x80){
        outb(RTC_STATUS_A,CMOS_INDEXPORT);
        update = inb(CMOS_DATAPORT);
        }
        cli();
        outb(RTC_DAY_MONTH,CMOS_INDEXPORT);
        seconds = inb(CMOS_DATAPORT);
        return seconds; 
}
