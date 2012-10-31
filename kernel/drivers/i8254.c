/**
 * @addtogroup Drivers
 * @{
 * @file i8254.c
 * @author Saad Talaat
 * @brief Intel 8253/8254 PIT(Programmable Interrupt Timer) Controller
 * @name Intel 8254 PIT
 * @{
 */ 

#include <drivers/i8254.h>
#include <drivers/i8259.h>
#include <arch/x86/x86.h>

/**
 * @brief Initiate the PIT to interrupt every time slice
 * @details
 * 	PIT is programmed to interrupt 20 times per second
 * which is relatively big time interval to schedule on
 *
 */
void
i8254_init(void)
{
	outb( MODE_RATEGEN | LATCH_ALL | SELECT_CNT0, PIT_MODE);
	outb( (PIT_TICKS / 20) & 0xf, PIT_CHANNEL0);
	outb( (PIT_TICKS / 20) >> 8 , PIT_CHANNEL0);
	i8259_unmask_irq(OCW1_PIT);

}


/**
 * @}
 * @}
 */
