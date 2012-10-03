//cc -c init_elf.c -o init_elf.o
//ld init_elf.o -T script.ld 
#include <types.h>
#include <stdio.h>

int main()
{
	asm("xchg %bx,%bx");
	while(1);
	asm("xch %bx,%bx");
	//NOTHING
	return 0;
}
