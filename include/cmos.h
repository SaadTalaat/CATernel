 /** 
 * @file cmos.h
 * @author Menna Essa , Saad Talaat
 * @date 29/10/2011
 * @brief defines constants for RTC 
 * http://catreloaded.net
 */
#ifndef _CATERNEL_CMOS_H_
#define _CATERNEL_CMOS_H_
/* This header includes values and I/O ports for handling CMOS/RTC */
/* CMOS Registers */
#define CMOS_INDEXPORT 	0x70	/* the CMOS/RTC index register port */
#define CMOS_DATAPORT 	0x71	/* the CMOS/RTC data register port */

/* RTC Registers */
#define	RTC_SECONDS	0x0
#define RTC_ALRMSECOND	0x1
#define RTC_MINUTES	0x2
#define RTC_ALRMMINUTE	0x3
#define RTC_HOUR	0x4
#define RTC_ALRMHOUR	0x5
#define RTC_DAY_WEEK	0x6
#define RTC_DAY_MONTH	0x7
#define RTC_MONTH	0x8
#define RTC_YEAR	0x9
#define RTC_STATUS_A	0xA	/* the RTC Status register A */
#define RTC_STATUS_B	0xB	/* the RTC Status register B */


#define STAT_RUN	0x7F     /*values to be used in the Masking of the RTC_STATUS_B*/
#define STAT_HALT       0x80  
#define STAT_PER_INTR   0x40
#define STAT_ALRM_INTR  0x20
#define STAT_UPDT_INTR  0x10
#define STAT_SQRWV_INTR 0x08
#define STAT_CAL_BIN	0x04
#define STAT_CAL_BCD	0xFB     
#define STAT_CAL_HR24   0x02
#define STAT_CAL_HR12   0xFD     
#define STAT_DAY_LGHT   0x01


#define CMOS_STATUS_C	0xC
#define CMOS_STATUS_D	0xD
#define CMOS_STATUS_E	0xE	/* RTC diagnostic register E */
#define CMOS_STATUS_F	0xF	/* RTC Shut-down status register F */
#define CMOS_STATUS_10	0x10	/* RTC DRIVER TYPE for A: and B: register */
#define CMOS_FIXED_DISK	0x12	/* RTC Fixed disk drive type for drive 0 and drive 1 register */
#define CMOS_EQUIP_BYTE	0x14	/* RTC Equip byte */
#define CMOS_SYSBASE_LSB	0x15	/* LSB of System base memory in Kilo byte*/
#define CMOS_SYSBASE_MSB	0x16	/* MSB of ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define CMOS_EXTMEM_LSB	0x17	/* LSB of total extended memory in Kilo byte */
#define CMOS_EXTMEM_MSB	0x18	/* MSB of ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define CMOS_DRIVE_C	0x19	/* Drive C extention type */
#define CMOS_DRIVE_D	0x1A	/* Drive D extention type */

/* functions */
uint8_t cmos_set_power_stat(uint8_t stat);
uint8_t cmos_get_seconds(void);
uint32_t cmos_get_reg(uint8_t value);
#endif
