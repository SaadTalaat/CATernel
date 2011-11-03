/* Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */

#ifndef _CATERNEL_STDIO_H_
#define _CATERNEL_STDIO_H_
#include <stdarg.h>
/***********************************************************/
/*** This header includes the Standard I/O operations	****/
/*** for The screen and the keyboard, it handles an	****/
/*** Interactive session between the user and kernel	****/
/***********************************************************/
/* Implementation in the cli.c */
void	putchr(int);
int	getchar(void);
/* Implementation in printf.c */
int printk(const char*,...);
int vprintk(const char*,va_list);
int kvprintk(const char*,void (*func)(int,int*),int *count,va_list ap);
void ksprintkn(void (*func)(int,int *),int *count,uintmax_t num,int base,int width,int padc);

char* readline(const char *);
#endif
