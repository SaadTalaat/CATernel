/**
 * @addtogroup Drivers
 * @{
 * @file clock.c
 * @author Saad Talaat
 * @brief Clock Low level controller. sets periodic interrupts and power state
 *
 * @name RTC Driver.
 * @{
 */
#include <arch/x86/x86.h>
#include <cmos.h>


void clock_enable_rtc(void) __attribute__((unused));
void
clock_enable_rtc(void)
{
	uint8_t stat;
	cmos_set_reg(RTC_STATUS_B, STAT_PER_INTR);
	cmos_set_reg(RTC_STATUS_A, 0x3);
	stat = cmos_get_reg(RTC_STATUS_B);
	printk("[*] Clock state: %x\n", stat);
	sti();
	asm("xchg %bx,%bx");
}


/**
 * @} @}
 */
