/**
 * @addtogroup MultiProcessors
 * @{
 * @file delay.c
 * @author Menna Essa
 * @brief Simple delay functions for synchornization 
 * @name Delay 
 * @{
 */ 

#include <arch/x86/x86.h>
#include <drivers/i8254.h>

/**
 * @brief useless loop to use while delaying
 * @param t number of loops
 * @return none
 */ 
void asm_delay_loop(uint32_t t)
{
        asm volatile ("movl %0 ,%%ecx\n\t"
		      "0:lahf\n\t"
		      "dec %%ecx\n\t"
		      "jnz 0b" : : "a"(t));
}

/**
 * @brief useless loop to calibrate cpu
 * @param  number of loops
 * @return none
 */ 
void asm_fake_loop(uint32_t t)
{

        asm volatile ("movl %0 ,%%ecx\n\t"
		      "0:lahf\n\t"
		      "dec %%ecx\n\t"
		      "jz 0b " : : "a"(t));
}
	

/**
 * @brief uinterruptable delay function
 * @param microseconds to wait 
 * @return none
 */ 
void delay(uint32_t microsec)
{
	cli();
	asm_delay_loop(microsec * delay_loop_const);
	sti();
}

/**
 * @}
 * @}
 */
  
