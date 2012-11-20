/**
 * @addtogroup CPU-Initialization
 * @{
 * @file processor.c
 * @author Saad Talaat
 * @brief Identify and initialize cpu structures
 * @name CPU-Identification
 * @{
 */
#include <arch/x86/processor.h>
#include <cpuid.h>
#include <arch/x86/mm/page.h>
char *cpu_vendors[3] =
{
	"Undefined\0",
	"Intel\0",
	"AMD\0"
};
cpu_t *cpu;
void
processor_printinfo();
void
processor_identify(void)
{
	cpuid_t id;
	cpu =allocate(sizeof(cpu), PAGESZ);
	cpuid(CPUID_INFO, &id);
	if(id.ebx == CPUID_INTEL_EBX_0 && id.ecx == CPUID_INTEL_ECX_0 &&
		id.edx == CPUID_INTEL_EDX_0)
	{
		cpu->vendor = VENDOR_INTEL;
	}
	else
	{
		cpu->vendor = 0;
	}
	cpuid(CPUID_VERSION, &id);
	*((uint32_t *)&cpu->version.fms) = id.eax;
	*((uint32_t *)&cpu->version.generic) = id.ebx;
	*((uint32_t *)&cpu->version.featured_ecx) = id.ecx;
	*((uint32_t *)&cpu->version.featured_edx) = id.edx;
	processor_printinfo();
	asm("xchg %bx,%bx");
}

void
processor_printinfo()
{
        printk("[*] CPU info:\n");
        printk("\tVendor   = %s\n",cpu_vendors[cpu->vendor]);
        printk("\tFamily   = %x\n",cpu->version.fms.family);
        printk("\tModel    = %x\n",cpu->version.fms.model);
        printk("\tStepping = %x\n",cpu->version.fms.stepping);
        printk("\tType     = %x\n",cpu->version.fms.type);
        printk("\tId       = %x\n",cpu->version.generic.id);
        printk("\tMax Id   = %x\n",cpu->version.generic.max_id);
        printk("\tH-Thread = %x\n",cpu->version.featured_edx.htt);
        printk("\tApic     = %x\n",cpu->version.featured_edx.apic);
        printk("\tx2Apic   = %x\n",cpu->version.featured_ecx.x2apic);
}

/**
 * @}
 * @}
 */
