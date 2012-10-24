/**
 * @addtogroup Main
 * @{
 */
/**
 * @file work_it_out.c
 * @author Saad Talaat
 * @date Thursday, 27/10/2011
 * @brief kernel device facilities initialization and set up start.
 * @name Mainflow.
 * @{
 *
 * After kernel is loaded into IP, it fetches into <b>_start</b>
 * function which makes a soft reboot by the bios, and reloads 
 * a new global descriptor table and resets segments. then just
 * before it jumps to the <b>work_it_out</b> function it sets 
 * stack up and jumps to earlier mentioned function. 
 *
 */

#include <types.h>
#include <cmos.h>
#include <video.h>
#include <cli.h>
#include <stdio.h>
#include <test.h>
#include <init.h>

/**
 * @fn void work_it_out(void);
 * @brief initializes screen and starts the bootup_process
 */
void
work_it_out(void)
{
//	console_init();
	console_clear();
	console_init();
	printk("CATernel is booting up...\n");
	bootup();
}

/**
 * @} @}
 */
