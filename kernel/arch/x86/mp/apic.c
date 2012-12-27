/**
 * @addtogroup Multiprocessors
 * @{
 * @file apic.c
 * @author Saad Talaat , Menna Essa
 * @date 18 November, 2012
 * @brief supporting xAPIC for MPs
 * @name APIC
 * @{
 */
#include <arch/x86/x86.h>
#include <arch/x86/processor.h>
#include <arch/x86/mp/apic.h>
#include <arch/x86/mm/page.h>
#include <memvals.h>
#include <arch/x86/mp/delay.h>

uint32_t vector_addr = 0x00022000;
uint32_t *lapic = (uint32_t *)(0xfee00000);

/**
 * @brief Soft Initiates the LAPIC  
 * @param none
 * @return none
 */
void
lapic_init(void)
{
	printk("[*] APIC: Local ID %x\n", lapic[LAID]);
	msr_lapic_enable();
	soft_lapic_enable();
	//asm("xchg %bx,%bx");
}

/** Enabling LAPIC */

/**
 * @brief Soft To enable the Local APIC to receive interrupts you also have to set bit 8 in the Spurious Interrupt Vector Register 
 * @param none
 * @return none
 */
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

/**
 * @brief APIC setting IA32_APIC_BASE Model Specific Register (MSR) 
 * @param none
 * @return none
 */
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

/*
void vector(void){asm("jmp %0"::"a"(0x7c00));}

void vector_test(void)
{
	map_segment_page(global_pgdir,vector_addr, PAGESZ, 
			0x22500000, PAGE_PRESENT|PAGE_WRITABLE);
}
*/

/**
 * @brief Sending Startup IPI to processor speicified with lapicid
 * @param icr Interrupt control register structure to set the options for the interrupt
 * @param lapicid the processors lapic id         
 * @return none
 */

void apic_s_ipi(icr_t icr , uint8_t lapicid)
{
	icr.delivery_mode = ICR_INIT;
	icr.dest_mode = PHYSICAL;
	icr.level = LEVEL_ASSERT;
	icr.shorthand = NO_SH;
	icr.trigger_mode = TRIGMOD_LEVEL;
	icr.vector = 0;
	icr.dest= lapicid;
	lapic[ICR_LOW] = icr.lo;
	//Wait 10ms 
	delay(10000);
	uint8_t j;
	if (!is_82489DX_apic(lapic[LAVR])) 
		{ j=1;} else {j=2;}
	unsigned int i;
	for (i = 0; i < j; i++) {
	icr.lo = lapic[ICR_LOW];
	icr.vector = 0 ;//trampoline function needs to be here
	icr.delivery_mode = ICR_SIPI;
	icr.dest_mode = PHYSICAL;
	icr.level = LEVEL_ASSERT;
	icr.shorthand = NO_SH;
	icr.trigger_mode = TRIGMOD_LEVEL;
	icr.dest=lapicid;
	lapic[ICR_LOW] = icr.lo;
	//Wait 200 us
	delay(200);
	apic_read_errors();
	//asm("xchg %bx,%bx");

	}
}

/**
 * @brief Sending INIT IPI to processor speicified with lapicid
 * @param lapicid the processors lapic id         
 * @return none
 */
void apic_init_ipi(uint8_t lapicid)
{
	icr_t icr;
	icr.lo = lapic[ICR_LOW];
	icr.hi = lapic[ICR_HIGH];	
	icr.delivery_mode = ICR_INIT;
	icr.dest_mode = PHYSICAL;
	icr.level = LEVEL_ASSERT;
	icr.trigger_mode = TRIGMOD_LEVEL;
	icr.shorthand = NO_SH;
	icr.vector = 0;
	icr.dest =lapicid; //broadcast? 0xFH for Pentium and P6 0xFFH for Pentium 4 and Intel Xeon processors.
	lapic[ICR_LOW] = icr.hi;
	lapic[ICR_HIGH] = icr.lo;	
	//According to MP Specification, 20us should be enough to deliver the IPI.
	uint32_t i;
	delay(20);
	apic_read_errors();
	asm("xchg %bx,%bx");
	apic_s_ipi(icr ,lapicid);
}

/**
 * @brief Reads error field in lapic 
 * @param none
 * @return none
 */ 

void apic_read_errors(void)
{
	uint32_t error = lapic[ERR];
	printk("[*] APIC Error [%p] : %x \n" ,lapic, error); 
}

/**
 * @}
 * @}
 */
