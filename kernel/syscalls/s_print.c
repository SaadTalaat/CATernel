/**
 * @addtogroup Interrupts-and-Syscalls
 * @{
 * @file s_print.c
 * @author Menna Essa
 * @date Thursday 25/10/2012
 * @brief The actual printf procedure -to be implemented- .
 * @name  Print procedure .
 * @{
 *
 */
#include <types.h>
#include <arch/x86/x86.h>
#include <syscalls/syscalls.h>
/**
 *@fn int32_t sys_printf(char * str)
 *@param str : format string to print
 *@return exit status. 
 *@brief The actual print procedure requested by the printf system call.  
 */
int32_t sys_printf(char * str)
{
	printk("Print called , to be implemented \n");
	return 0;
}
 /** @} @} */
