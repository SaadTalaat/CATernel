#include <types.h>
#include <arch/x86/x86.h>
#include <cpuid.h>

struct cpuid_regs{
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
};
uint32_t cpuid_get_ebx(void);
uint32_t cpuid_get_ecx(void);
uint32_t cpuid_get_edx(void);
uint32_t cpuid_get_eax(void);
static inline void make_cpuid(uint32_t *eax,uint32_t *ebx,uint32_t *ecx,uint32_t *edx);

uint32_t
set_eax(uint32_t value){
	return value;
}
uint32_t
cpuid_print(uint32_t ax_value,uint32_t operation){
	char *str;
	uint32_t a,b,c,d;
	struct cpuid_regs *regs;
	regs->eax = operation;
	make_cpuid(&regs->eax,&regs->ebx,&regs->ecx,&regs->edx);
	a = regs->eax;
	b = regs->ebx;
	c = regs->ecx;
	d = regs->edx;
/*	switch(operation){
		case CPUID_INFO:
			reg = cpuid_get_ebx();
			if (reg == 0x756e6547){
				printk("GenuineIntel Processor\n");
			}
			break;
		case CPUID_VERSION:
			printk("dug %x\n",cpuid_get_eax());
			break;
	}*/
}
static inline void
make_cpuid(uint32_t *eax,uint32_t *ebx,uint32_t *ecx,uint32_t *edx){
	*eax = 0;
	*ecx = 1;
	asm volatile("cpuid":
			"=a" (*eax),
			"=b" (*ebx),
			"=c" (*ecx),
			"=d" (*edx):
			"0"(*eax),
			"2"(*ecx));

}
/*
inline uint32_t
cpuid_get_ebx(void){
	uint32_t data;
//	asm("xor %eax,%eax");
//	asm("mov %0,%%eax"::"a"(&(data)));
	asm volatile("mov %%ebx,(%0)"::"a"(&(data)));
	return data;
}
inline uint32_t
cpuid_get_ecx(void){
	uint32_t data1;
//	asm("xor %eax,%eax");
//	asm("mov %0,%%eax"::"a"(&(data1)));
	asm volatile("mov %%ecx,(%0)"::"a"(&(data1)));
	return data1;
}
inline uint32_t
cpuid_get_edx(void){
	uint32_t data2;
//	asm("xor %eax,%eax");
//	asm("mov %0,%%eax"::"a"(&(data2)));
	asm volatile("mov %%edx,(%0)":: "a"(&(data2)));
	return data2;
}
inline uint32_t
cpuid_get_eax(void){
	uint32_t data;
	asm volatile("mov %%eax,(%0)"::"a"(data));
	return data;

}*/
