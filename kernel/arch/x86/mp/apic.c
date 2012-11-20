/**
 * @addtogroup Multiprocessors
 * @{
 * @file apic.c
 * @author Saad Talaat
 * @date 18 November, 2012
 * @brief supporting xAPIC for MPs
 * @name APIC
 * @{
 */
#include <arch/x86/x86.h>
#include <arch/x86/processor.h>
#include <arch/x86/mp/apic.h>

uint32_t *lapic = (uint32_t *)(0xfee00000);
void
lapic_init(void)
{
	printk("[*] APIC: Local ID %x\n", lapic[LAID]);
	msr_lapic_enable();
	soft_lapic_enable();
	asm("xchg %bx,%bx");
}

/** Enabling LAPIC */
void
soft_lapic_enable(void)
{
	uint32_t flag;
	if(!cpu->version.featured_edx.apic)
	{
		printk("[*] APIC: Not Supported\n");
		return;
	}

	flag = lapic[SVR] & (1 << 8);	/* 8 = enable/disable bit in SPUR */
	if(flag)
	{
		printk("[*] APIC%d: Soft enabled\n", cpu->version.generic.id);
		return;
	}
	else
	{
		printk("[*] APIC%d: Soft disabled\n", cpu->version.generic.id);
		printk("[*] APIC%d: Soft enabling\n", cpu->version.generic.id);
		lapic[SVR] |= (1 << 8);
		return;
	}
}
void
msr_lapic_enable(void)
{
	uint32_t msr0_31, msr32_63;
	uint32_t flag;
	if(!cpu->version.featured_edx.apic)
	{
		printk("[*] APIC: Not Supported\n");
		return;
	}
	read_msr(IA32_APIC_BASE, &msr32_63, &msr0_31);
	flag =(msr0_31) &  (1 << IA32_APIC_BASE_ENABLE);
	if(flag)
	{
		printk("[*] APIC%d: MSR enabled\n",cpu->version.generic.id);
		return;
	}
	else
	{
		printk("[*] APIC%d: MSR Enabling\n",cpu->version.generic.id);
		msr0_31 |= (1 << IA32_APIC_BASE_ENABLE);
		write_msr(IA32_APIC_BASE, msr32_63, msr0_31);
	}
	
}
/**
 * @}
 * @}
 */
