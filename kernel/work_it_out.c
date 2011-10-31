#include <types.h>
#include <cmos.h>

void
work_it_out(void){
	cmos_set_power_stat(STAT_CAL_HR24);
	cmos_get_time(RTC_HOUR);
		
	while(1);
}
