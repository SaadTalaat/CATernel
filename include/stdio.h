/**
 * @file stdio.h
 * @author Saad Talaat
 * @brief Standard Input Output operations
 * @details 
 * This header includes the Standard I/O operations
 * for The screen and the keyboard, it handles an 
 * Interactive session between the user and kernel
 *
 */

#ifndef _CATERNEL_STDIO_H_
#define _CATERNEL_STDIO_H_
#include <stdarg.h>


/**
 * @fn void putchr(int);
 * @param int char to be written to display buffer.
 * @brief writes a character to screen.
 * 
 * @fn int getchar(void);
 * @return int char read from keyboard.
 * @brief recives a character from keyboard and write it to screen and buffer.
 */
void	putchr(int);
int	getchar(void);


int printk(const char*,...);
int vprintk(const char*,va_list);
int kvprintk(const char*,void (*func)(int,int*),int *count,va_list ap);
void ksprintkn(void (*func)(int,int *),int *count,uintmax_t num,int base,int width,int padc);


/**
 * @fn char *readline(const char*);
 * @param const char * character to write
 * @return the string read.
 * @brief reads a string from keyboard into screen.
 * @details It uses the put and get char functions
 * from the cli code. then keep on receiving characters
 * until [ENTER] is pressed.
 */
char* readline(const char *);
#endif
