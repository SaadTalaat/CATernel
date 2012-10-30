#include <types.h>
#include <arch/x86/x86.h>
#include <cmos.h>
/**
 * @addtogroup Drivers
 * @{
 * @file cmos.c
 * @author Menna Essa
 * @date 31/10/2011
 * @brief CMOS Driver.
 * @name CMOS Driver.
 * @{
 * 
 *  supports status A and B Power options , Refer to boch's Ports.lst for details.
 */


/**
 * @fn cmos_set_power_stat(uint8_t stat);
 * @brief supports CMOS Status B power options.
 * @param uint8_t stat , the value of the option to be set.
 * @return uint8_t New_Stat , the CMOS new power status.
 *
 */

uint8_t cmos_set_power_stat(uint8_t stat)
{
	uint8_t update , New_Stat;
       /*Wait until CMOS is free , that is it's not being updated "0x80 status A" */
 	do{
		outb(RTC_STATUS_A,CMOS_INDEXPORT);
		update = inb(CMOS_DATAPORT);
	}while(update == 0x80);
	cli();	
 	outb(RTC_STATUS_B | 0x80,CMOS_INDEXPORT);
	/*read the initial CMOS state*/
	New_Stat=inb(CMOS_DATAPORT); 
 	/*those 3 sets the respective bit to zero so we mask with AND*/
 	if(stat==STAT_RUN || stat==STAT_CAL_BCD || stat==STAT_CAL_HR12) 
	  New_Stat &= stat;
 	else
	  New_Stat |= stat;
 	outb(RTC_STATUS_B | 0x80 ,CMOS_INDEXPORT);
 	/*write the new status to the port.*/
 	outb(New_Stat,CMOS_DATAPORT);
 	return New_Stat;	
}


/**
 * @fn uint32_t cmos_get_reg(uint8_t value);
 * @brief Gets RTC registers values.
 * @param uint8_t value , the register to be read.
 * @return uint32_t val , the value of the register.
 *
 */

uint32_t cmos_get_reg(uint8_t value){
	uint32_t val;
	uint8_t update;
	//check status
	do {
		outb(RTC_STATUS_A,CMOS_INDEXPORT);
		update = inb(CMOS_DATAPORT);
	}while (update == 0x80);
	cli();
	//get the value 
	outb(value,CMOS_INDEXPORT);
	val = inb(CMOS_DATAPORT);
	sti();
	return val;
}

/**
 * @fn uint32_t cmos_set_reg(uint8_t index, uint8_t value);
 * @brief set RTC registers values.
 * @param uint8_t value , the value to set register to.
 * @param uint8_t index,  index of the register.
 * @return uint32_t val , the value of the register.
 *
 */

uint32_t cmos_set_reg(uint8_t index, uint8_t value){
	uint32_t val;
	uint8_t update;
	//check status
	cli();
	//get the value
	outb(index | 0x80,CMOS_INDEXPORT);
	update = inb(CMOS_DATAPORT);
	outb(index  | 0x80 ,CMOS_INDEXPORT);
	outb(value | update,CMOS_DATAPORT);
	sti();
	return val;
}

/**
 * @}
 * @}
 */
