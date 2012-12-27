/**
 * @addtogroup Drivers
 * @{
 * @file i8254.c
 * @author Saad Talaat , Menna Essa
 * @brief Intel 8253/8254 PIT(Programmable Interrupt Timer) Controller
 * @name Intel 8254 PIT
 * @{
 */ 

#include <drivers/i8254.h>
#include <drivers/i8259.h>
#include <arch/x86/x86.h>
#include <arch/x86/mp/delay.h>

#define MAGIC_NUMBER  1194
#define LOOPS  150000

uint32_t delay_loop_const;
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
 * @brief calibrates the number of loops per 1 ms
 * @param none
 * @return none
 */ 

void i8254_calibrate_delay_loop(void)
{
	/*
	 * One-shot timer. Count-down from 0xffff at 1193180Hz
	 * MAGIC_NUMBER is the magic value for 1us. kudos to helenOS
	 */
	outb(LATCH_ALL,PIT_MODE);
	outb(0xff,PIT_CHANNEL0);
	outb(0xff,PIT_CHANNEL0);	
	uint8_t not_ok;
	uint32_t t1;
	uint32_t t2;	
	do {
		/* will read both status and count */
		outb(0xc2,PIT_MODE);
		not_ok = (uint8_t) ((inb(PIT_CHANNEL0) >> 6) & 1);
		t1 = inb(PIT_CHANNEL0);
		t1 |= inb(PIT_CHANNEL0) << 8;
	} while (not_ok);
	asm_delay_loop(LOOPS);	
	outb(0xd2,PIT_MODE);
	t2 = inb(PIT_CHANNEL0);
	t2 |= inb(PIT_CHANNEL0) << 8;	
	/*
	 * We want to determine the overhead of the calibrating mechanism.
	 */
	outb(0xd2,PIT_MODE);
	uint32_t o1 = inb(PIT_CHANNEL0);
	o1 |= inb(PIT_CHANNEL0) << 8;
	asm_fake_loop(LOOPS);
	outb(0xd2,PIT_MODE);
	uint32_t o2 = inb(PIT_CHANNEL0);
	o2 |= inb(PIT_CHANNEL0) << 8;
	delay_loop_const =
	    ((MAGIC_NUMBER * LOOPS) / 1000) / ((t1 - t2) - (o1 - o2)) +
	    (((MAGIC_NUMBER * LOOPS) / 1000) % ((t1 - t2) - (o1 - o2)) ? 1 : 0);
	printk("[*] Delay loop calibration => %d \n" , delay_loop_const); 
	//asm("xchg %bx,%bx");	
	return;
}


/**
 * @}
 * @}
 */
