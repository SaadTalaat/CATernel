/* Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */

#ifndef _CATERNEL_CLI_H_
#define _CATERNEL_CLI_H_
#define MAXBUFSIZE	512

/** 
 * @fn void console_init(void);
 * @brief initalizes console.
 * @details Initalizes the display device and sets cursor positions
 * to zero.
 *
 *
 * @fn void console_interrupt(int (*)(void));
 * @brief reads a char from a input device interrupt.
 * @paramter int (*intr)(void) interrupt handler.
 * @details This function takes the input device interrupt
 * handler as a paramter and reads a character from it into
 * the console buffer.
 *
 *
 * @fn int console_getc(void);
 * @return int c Character read.
 * @brief reads a character from an input device [keyboard].
 * @details It issues a keyboard interrupt which issues a 
 * console interrupt, and wait for a character to be read.
 *
 *
 * @fn void console_putc(int);
 * @paramter int c character to print.
 * @brief writes a character to screen
 * @detail wraps cga_putc function
 *
 */
void	console_init(void);
void	console_interrupt(int (*intr)(void));
int	console_getc(void);
void	console_putc(int c);
static struct {
	uint8_t buf[MAXBUFSIZE];
	uint32_t rpos;
	uint32_t wpos;
} cons;

#endif
