#include <types.h>
#include <test.h>
#include <sys.h>
#include <arch/x86/x86.h>
#include <arch/x86/cpu_state.h>
#include <syscalls/syscalls.h>

int32_t sys_call_table[] =  { sys_printf };
int32_t n_calls = sizeof(sys_call_table)/sizeof(int32_t); 
int32_t s_errno;
int32_t map_syscall(cpu_state_t *cpu_state)
{
	printk("System call issued \n");
	if(cpu_state->eax > n_calls );
	{
		printk("Bad system call code \n");
		return 1;
	}
	s_errno=sys_call_table[cpu_state->eax](cpu_state->ecx);
	if(ret < 0 ) {return -s_errno;}  //error code		
	else {return 0;}
	       	
}

