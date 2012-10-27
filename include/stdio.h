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


/**
 * @fn int printk(const char*, ...)
 * @param const char* format string, an ideal format string.
 * @param stdarg argument list, variable referenced.
 * @return int number of chars written.
 * @brief An ideal printing function.
 * @details
 * A printk function is simply a wrapper to the vprintk function.
 * It reads the paramter list and pass it to vprintk.
 *
 *
 * @fn int vprintk(const char*, va_list);
 * @param const char* format string.
 * @param va_list paramter list.
 * @return int number of chars written.
 * @brief passes arugments to kvprintk after tokenizing them
 *
 *
 * @fn int kvprintk(const char*, void (*)(int,int*), int *, va_list);
 * @param const char* format string.
 * @param void (*)(int,int*) a char printing function.
 * @param int* reference of count variable, to store count in.
 * @param va_list the list of paramters passed by printk->vprintk
 * @return count of chars written
 * @brief A BSD like printing function.
 *
 *
 * @fn void ksprintkn(void (*func)(int,int *),int *count,uintmax_t num,int base,int width,int padc)
 * @param void (*)(int, int*) a char printing function.
 * @param int* reference of the count variable, to store new count in.
 * @param uintmax_t the number to print
 * @param int base of the number.
 * @param int digit width to print according to.
 * @param int the padding character to print to pad a number.
 * @brief Prints numbers.
 * @details this function converts numbers into their character representation
 * the function recursively calls itself to print lower digits first.
 */
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
