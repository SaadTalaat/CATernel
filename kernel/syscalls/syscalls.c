/**
 * @addtogroup Interrupts-and-Syscalls
 * @{
 * @file syscalls.c
 * @author Menna Essa
 * @date Thursday 25/10/2012
 * @brief maps calls into their respective handlers.
 * @name System calls handler .
 * @{
 *
 */

#include <types.h>
#include <test.h>
#include <sys.h>
#include <arch/x86/x86.h>
#include <arch/x86/cpu_state.h>
#include <syscalls/syscalls.h>

/**
 * @brief Declare system calls table with pointers to calls handlers
 */
fnptr_t sys_call_table[] =  { sys_printf };
/**
 * @brief keeps the size of the table for boundary checking.
 */
int32_t n_calls = sizeof(sys_call_table)/sizeof(int32_t); 
/**
 * @brief Keeps error codes to return to the caller.
 */
int32_t s_errno=0;
/**
 *@fn int32_t map_syscall(cpu_state_t *cpu_state);
 *@param cpu_state : pointer to cpu_state_t structure
 *@return exit status / error code.
 *@brief maps the system call by offseting the sys_call_table structure using the offset in eax register.
 */
int32_t map_syscall(cpu_state_t *cpu_state)
{
	printk("System call issued \n");
	if(cpu_state->eax > n_calls );
	{
		printk("Bad system call code \n");
		return 1;
	}
	s_errno= (sys_call_table [cpu_state->eax])((char *)cpu_state->ecx);
	if(s_errno < 0 ) {return -s_errno;}  //error code		
	else {return 0;}
	       	
}

