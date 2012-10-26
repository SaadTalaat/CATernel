/**
 * @addtogroup Drivers
 * @{
 * @file i8259.c
 * @author Saad Talaat
 * @brief Intel 8259 PIC(Programmable Interrupt Controller) Low Level Driver.
 * @name i8259-PIC Driver.
 * @{
 *
 */
#include <arch/x86/x86.h>
#include <drivers/i8259.h>
/**
 * @fn void i8259_init(void);
 * @brief Initializing the PIC to work under Protected mode.
 * @details
 * Since PIC was designed to work under real mode, it worked under
 * the IVT which's IRQ indexes conflict with the default Exceptions
 * in IDT defined by Intel, so we need to offset the default IRQs
 * with 0x20 slot or whatever it requires to avoid conflict with
 * the Intel default Exceptions.
 *
 * PIC Initialization is done by issuing four Initialization 
 * command words.
 * ICW1 = 0x11 both ICW4 Needed and ICW1 ISSUED Flags are set.
 * ICW2 = the IRQ Base offset.
 * ICW3 = The PIC Slave attachment situation
 * ICW4 = Additional info.
 * 
 */
void
i8259_init(void)
{

	uint8_t micw1, sicw1, micw2, sicw2, micw3, sicw3, micw4, sicw4;

	/**
	 * 00010001 Flags.
	 * ICW1 ISSUED - CASCADE MODE - EDGE TRIGGERED MODE - ICW4 NEEDED
	 */
	micw1 = sicw1 = ICW1_ISSUED | ICW1_ICW4NEEDED;
	micw2 = IRQ_BASE;
	sicw2 = IRQ_BASE + 8;
	micw3 = PIC_SLAVE_2;
	sicw3 = SLAVE_ON_2;
	/**
	 * 00000001 Flags.
	 * 8086/8088 MODE - NORMAL EOI - NON BUFFERED MODE - 
	 * NO SPECIAL FULL NESTED MODE
	 */
	micw4 = sicw4 = ICW4_86_88;

	/**
	 * Output flags to PICs
	 */
	outb(micw1, PIC_MASTER_ICW);
	outb(micw2, PIC_MASTER_OCW);
	outb(micw3, PIC_MASTER_OCW);
	outb(micw4, PIC_MASTER_OCW);
	
	outb(sicw1, PIC_SLAVE_ICW);
	outb(sicw2, PIC_SLAVE_OCW);
	outb(sicw3, PIC_SLAVE_OCW);
	outb(sicw4, PIC_SLAVE_OCW);
}

/**
 * @fn void i8259_disable(void);
 * @breif Disables the PIC IRQs.
 * @details
 * This function is mostly used before setting up APIC to usage.
 */
void
i8259_disable(void)
{
	outb(OCW1_INTRS_OFF, PIC_MASTER_OCW);
	outb(OCW1_INTRS_OFF, PIC_SLAVE_OCW);

}

/**
 * @fn void i8259_mask_irq(uint8_t irq);
 * @param uint8_t irq, the IRQ FLAG to set (e.g. OCW1_KYBRD_OFF)
 * @brief disables a specific IRQ
 * 
 */
void
i8259_mask_irq(uint8_t irq)
{
	uint8_t old_mask;
	if(irq >= (1<<8))
	{
		old_mask = inb(PIC_SLAVE_OCW);
		outb((irq >> 8) | old_mask, PIC_SLAVE_OCW);
	}
	else 
	{
		old_mask = inb(PIC_MASTER_OCW);
		outb(irq | old_mask, PIC_MASTER_OCW);
	}
}

/**
 * @fn void i8259_unmask_irq(uint8_t irq);
 * @param uint8_t irq, the IRQ flag to unset.
 * @brief enables a specific IRQ
 *
 */
void
i8259_unmask_irq(uint8_t irq)
{
	uint8_t old_mask;
	if(irq >= (1<<8))
	{
		old_mask = inb(PIC_SLAVE_OCW);
		outb( ~(irq >> 8) & old_mask, PIC_SLAVE_OCW);
	}
	else
	{
		old_mask = inb(PIC_MASTER_OCW);
		outb( ~(irq) & old_mask, PIC_MASTER_OCW);
	}
}

/**
 * @fn uint16_t i8259_read_isr(void);
 * @return returns the In service interrupt mask
 * @brief get the in service interrupt.
 */
uint16_t
i8259_read_isr(void)
{
	outb(OCW3_READ_ISR, PIC_MASTER_ICW);
	outb(OCW3_READ_ISR, PIC_SLAVE_ICW);
	return ( (inb(PIC_MASTER_ICW))|(inb(PIC_SLAVE_OCW) << 8));
}

/**
 * @fn uint16_t i8259_read_irr(void);
 * @return returns the Interrupt request register.
 * @brief reads the IRR register which holds raised interrupt priority
 *
 */
uint16_t
i8259_read_irr(void)
{
	outb(OCW3_READ_IRR, PIC_MASTER_ICW);
	outb(OCW3_READ_IRR, PIC_SLAVE_ICW);
	return ( (inb(PIC_MASTER_ICW))|(inb(PIC_SLAVE_OCW) << 8));
}

/**
 * @fn void i8259_eoi(uint8_t irq);
 * @param uint8_t irq, the Interrupt index that has ended.
 * @brief End of interrupt instruction, used for ISRs.
 * @details
 * In interrupt service routines an [end of interrupt] must
 * be issued after fullfilling the service routine.
 * In order to avoid issuing an eoi for an interrupt raised
 * on slave PIC by an ISR of an interrupt in service on 
 * master PIC, an eoi is issued for a single PIC at a time.
 */
void
i8259_eoi(uint8_t irq)
{
	
	if(irq >= 8 && irq < 16)
		outb(PIC_EOI, PIC_SLAVE_ICW);
	else
		outb(PIC_EOI, PIC_MASTER_ICW);
}

/**
 * @}
 * @}
 */
