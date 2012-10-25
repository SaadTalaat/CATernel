#include <types.h>
#include <arch/x86/x86.h>
#include <syscalls.h>

int32_t sys_printf(char * str)
{
	printk("Print called , to be implemented \n");
	return 0;
}
