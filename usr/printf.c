#include <types.h>
#include <sys.h>
void printf(char * str)
{
//sets cpu_state registers eax <-0 ebx<-1 ecx<-stringformat and issues a syscall interrupt
	asm("movl %0,%%eax" :: "a"(S_PRINTF));
	asm("movl %0,%%ebx":: "a"(1));
	asm("movl %0,%%ecx": :"=g"(str));
	asm("int %0" : :"a"(0x30));
}
