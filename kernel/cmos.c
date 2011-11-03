#include <types.h>
#include <x86.h>
#include <cmos.h>


/* 
 *  Status B Power options Refer to Ports.lt for details
 */
uint8_t cmos_set_power_stat(uint8_t stat)
{
uint8_t update , New_Stat;
//check that the register is not in update mode
 while(update == 0x80){
	outb(RTC_STATUS_A,CMOS_INDEXPORT);
	update = inb(CMOS_DATAPORT);
	}	
	cli();	
 	outb(RTC_STATUS_B,CMOS_INDEXPORT);
        New_Stat=inb(CMOS_DATAPORT); //read initial state
 //those 3 sets the respective bit to there so we mask with AND
 if(stat==STAT_RUN || stat==STAT_CAL_BCD || stat==STAT_CAL_HR12) 
	  New_Stat &= stat;
 else
	  New_Stat |= stat;
 outb(RTC_STATUS_B,CMOS_INDEXPORT);
 outb(New_Stat,CMOS_DATAPORT);//write the new status to the port.

 return New_Stat;
 //for debugging the function will be void later.
	
}


//Get RTC Values
uint8_t
cmos_get_time(uint8_t value) //value holds whether it's day,month,seconds,etc..
{
        uint8_t time;
	uint8_t update;
	//check status
	while(update == 0x80){
	outb(RTC_STATUS_A,CMOS_INDEXPORT);
	update = inb(CMOS_DATAPORT);
	}
	cli();
	//get the value 
	outb(value,CMOS_INDEXPORT);
	time = inb(CMOS_DATAPORT);
	return time;
}


