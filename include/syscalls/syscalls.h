#include <arch/x86/cpu_state.h>
#ifndef _CATRNEL_SYSCALLS_H
#define _CATRNEL_SYSCALLS_H
extern int32_t map_syscall(cpu_state_t *cpu_state);

/*system calls*/
extern int32_t sys_printf(char * str);

#endif 
