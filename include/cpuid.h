#ifndef _CATERNEL_CPUID_H_
#define _CATERNEL_CPUID_H_

/* Inputs */
#define CPUID_INFO	0x0
#define	CPUID_VERSION	0x1
#define CPUID_CACHE	0x4
/* Ouput */
/* CPUID_INFO */
#define CPUID_INTEL_EBX_0 0x756e6547
#define CPUID_INTEL_ECX_0 0x6c65746e
#define CPUID_INTEL_EDX_0 0x49656e69
#define CPUID_P4HT_0 0x49656e69 /* Intel Pentium 4 support Hyperthreading */

/* CPUID_VERSION */ 


typedef struct {

	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
} cpuid_t;

static inline void cpuid(uint32_t eax_initial,cpuid_t *c)
{
	__asm __volatile("cpuid" : "=a"(c->eax),"=b"(c->ebx),"=c"(c->ecx),"=d"(c->edx):"a"(eax_initial));
}
#endif
