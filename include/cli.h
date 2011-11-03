/* Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */

#ifndef _CATERNEL_CLI_H_
#define _CATERNEL_CLI_H_
#define MAXBUFSIZE	512
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
