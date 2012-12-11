/**
 * @file processor.h
 * @author Saad Talaat
 * @date Thursday 24/11/2011
 * @brief contains constants that are used by Intel x86
 * @details
 * The header contains Control registers constants and 
 * EFLAGS masks.
 */
#ifndef _CATERNEL_PROCESSOR_H_
#define _CATERNEL_PROCESSOR_H_
#include <types.h>
#include <cpuid.h>
/* Control Register 0 flags */
#define X86_CR0_PE	0x00000001	/* Protection Enable */
#define X86_CR0_MP	0x00000002	/* Monitor Coprocessor */
#define X86_CR0_EM	0x00000004	/* Emulation */
#define X86_CR0_TS	0x00000008	/* Task Switched */
#define X86_CR0_ET	0x00000010	/* Extension type */
#define X86_CR0_NE	0x00000020	/* Numeric error */
#define X86_CR0_WP	0x00010000	/* Write Protect */
#define X86_CR0_AM	0x00040000	/* Alignment Mask */
#define X86_CR0_NW	0x20000000	/* Not writable through */
#define X86_CR0_CD	0x40000000	/* Cache disabled */
#define X86_CR0_PG	0x80000000	/* Paging enable */

/* Control Register 3 flags */
#define X86_CR3_PWT	0x00000008	/* Page Write Through */
#define X86_CR3_PCD	0x00000010	/* Page Cache Disabled */

/* Control Register 4 flags */
#define X86_CR4_VME	0x00000001	/* Enable VM86 Extension */
#define X86_CR4_PVI	0x00000002	/* Virtual interrupts flag enable */
#define X86_CR4_TSD	0x00000004	/* Disable time stamp at ipl 3 */
#define X86_CR4_DE	0x00000008	/* Enable Debugging extensions */
#define X86_CR4_PSE	0x00000010	/* Enable Page Size extension */
#define X86_CR4_PAE	0x00000020	/* Enable Physical address extension */
#define X86_CR4_MCE	0x00000040	/* Machine Chech enable */
#define X86_CR4_PGE	0x00000080	/* Enable Global Pages */
#define X86_CR4_PCE	0x00000100	/* Enable Performance Counters at ipl 3*/
#define X86_CR4_OSFXSR	0x00000200	/* Enable Fast FPU save and restore */
#define X86_CR4_OSXMMEXCPT	0x00000400	/* Enable unmasked SSE exceptions */
#define X86_CR4_VMXE	0x00002000	/* Enable VMX virtualization */
#define X86_CR4_OSXSAVE	0x00004000	/* Enable Xsave and xrestore */

/* Control Register 8 flags */
#define X86_CR8_TPR	0x0000000F	/* Task Priority Register */

/* EFLAGS */

#define FLAG_CF	0x1
#define	FLAG_PF	0x4
#define FLAG_AF	0x10
#define FLAG_ZF	0x40
#define FLAG_SF	0x80
#define FLAG_TF	0x100
#define FLAG_IF	0x200
#define FLAG_DF	0x400
#define FLAG_OF	0x800
#define FLAG_IOPL	0x1000
#define FLAG_NT	0x2000
#define FLAG_RF	0x4000
#define FLAG_VM	0x8000
#define FLAG_AC	0x10000
#define FLAG_VIF 0x20000
#define FLAG_VIP 0x40000
#define FLAG_ID	0x80000

/* Vendors */
#define VENDOR_INTEL 0x1
#define VENDOR_AMD 0x2

struct cpu_version{
	struct {
		unsigned stepping:4;
		unsigned model:4;
		unsigned family:4;
		unsigned type:2;
		unsigned ss:2;
		unsigned exmodel:4;
		unsigned exfamily:8;
		unsigned sss:4;
		} fms;
	struct{
		char brand;
		char cache;
		char max_id;
		char id;
		} generic;
	struct{
		unsigned sse3:1;
		unsigned :4;
		unsigned vmx:1;
		unsigned smx:1;
		unsigned est:1;
		unsigned tm2:1;
		unsigned ssse3:1;
		unsigned :7;
		unsigned pcid:1;
		unsigned dca:1;
		unsigned sse4_1:1;
		unsigned sse4_2:1;
		unsigned x2apic:1;
		unsigned :10;
		} __attribute__((packed)) featured_ecx;
	struct {
		unsigned fpu:1;
		unsigned vme:1;
		unsigned de:1;
		unsigned pse:1;
		unsigned :2;
		unsigned pae:1;
		unsigned :2;
		unsigned apic:1;
		unsigned :1;
		unsigned sep:1;
		unsigned pge:1;
		unsigned mca:1;
		unsigned cmov:1;
		unsigned pat:1;
		unsigned :5;
		unsigned acpi:1;
		unsigned mmx:1;
		unsigned fxsr:1;
		unsigned sse:1;
		unsigned sse2:1;
		unsigned ss:1;
		unsigned htt:1;
		unsigned tm:1;
		unsigned :1;
		unsigned pbe:1;
		} __attribute__((packed)) featured_edx;
	
}__attribute__((packed));
struct cpu_cache{
	uint32_t eax;
	struct {
		unsigned line_size:12;
		unsigned phy_line :10;
		unsigned assoc:10;
		} __attribute__((packed)) lpw;
	uint32_t sets;
	struct {
		unsigned write_back:1;
		unsigned inclusive:1;
		unsigned indexing:1;
		unsigned :29;
		} __attribute__((packed)) wcc;
	


};
typedef struct{
        uint32_t vendor;
        struct cpu_version version;
        struct cpu_cache cache;
} cpu_t;

extern char *cpu_vendors[];
extern cpu_t *cpu;



/* ============================ */
/** MEMORY**/
#define MFENCE	asm volatile("mfence");
#define LFENCE	asm volatile("lfence");
#define SFENCE	asm volatile("sfence");
#endif /* _CATERNEL_PROCESSOR_H_ */
