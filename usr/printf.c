/**
 * @addtogroup Interrupts-and-Syscalls

 * @{
 * @file printf.c
 * @author Menna Essa
 * @date Tuesday 25/10/2012
 * @brief Printf System call.
 * @name Printf call.
 * @{
 *
 */

#include <types.h>
#include <sys.h>
/**
 *@fn void printf(char * str);
 *@param str : format string to print
 *@brief sets cpu_state registers eax <-0 ebx<-1 ecx<-stringformat and issues a syscall interrupt
 */
void printf(char * str)
{

	asm("movl %0,%%eax" :: "a"(S_PRINTF));
	asm("movl %0,%%ebx":: "a"(1));
	asm("movl %0,%%ecx": :"=g"(str));
	asm("int %0" : :"a"(0x30));
}

 /** @} @} */
