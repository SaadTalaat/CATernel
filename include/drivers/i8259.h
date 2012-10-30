/**
 *
 * @file i8259.h
 * @brief Intel 8259 PIC header file
 * @author Saad Talaat
 *
 */

#ifndef _CATERNEL_DRIVER_8259_H_
#define _CATERNEL_DRIVER_8259_H_
#include <types.h>

#define PIC_MASTER_ICW	0x20
#define PIC_MASTER_OCW	0x21
#define PIC_SLAVE_ICW	0xA0
#define PIC_SLAVE_OCW	0xA1

// ICW1 Constants
#define ICW1_ICW4NEEDED	0x1
#define ICW1_SINGLEMODE	0x2
#define ICW1_4BTVECTORS	0x4
#define ICW1_LEVELMODE	0x8
#define	ICW1_ISSUED	0x10
// ICW3 Constants
#define PIC_SLAVE_0	0x1
#define PIC_SLAVE_1	0x2
#define PIC_SLAVE_2	0x4
#define PIC_SLAVE_3	0x8
#define PIC_SLAVE_4	0x10
#define PIC_SLAVE_5	0x20
#define PIC_SLAVE_6	0x40
#define PIC_SLAVE_7	0x80

#define SLAVE_ON_0	0x0
#define SLAVE_ON_1	0x1
#define SLAVE_ON_2	0x2
#define SLAVE_ON_3	0x3
#define SLAVE_ON_4	0x4
#define SLAVE_ON_5	0x5
#define SLAVE_ON_6	0x6
#define SLAVE_ON_7	0x7

// ICW4 Constants
#define ICW4_86_88	0x1
#define ICW4_AUTOEOI	0x2
#define ICW4_NONBUFFER	0x4
#define ICW4_SLAVE_BUF	0x8
#define	ICW4_MASTER_BUF	0xC
#define ICW4_FULLNESTED	0x10

// Exceptions and IRQs
#define EXC_BASE	0x0
#define EXC_SIZE	0x20
#define IRQ_BASE	(EXC_BASE+EXC_SIZE)
#define IRQ_SIZE	0x10
#define IRQ_END		(IRQ_BASE+IRQ_SIZE)
#define INTR_EMPTY_BASE	IRQ_END
#define INTR_END	0xFF

// IRQ indexes
#define IRQ_PIT		0x0
#define IRQ_KBD		0x1
#define IRQ_VID		0x2
#define IRQ_SER1	0x3
#define IRQ_SER2	0x4
#define IRQ_LPT2	0x5
#define IRQ_FLPY	0x6
#define IRQ_SPUR	0x7
#define IRQ_RTC		0x8	//start of SLAVE PIC IRQs
#define IRQ_MOUSE	0xC
#define IRQ_FPU		0xE
#define IRQ_PRIHD	0xF
#define IRQ_SECHD	0x10


// OCW1 Constants
#define OCW1_PIT	0x1	// valid for Both RTC and PIT
#define OCW1_RTC	(OCW1_PIT << 8)	// valid for Both RTC and PIT
#define OCW1_KYBRD	0x2	// reserved on SLAVE
#define OCW1_VIDEO	0x4	// reserved on SLAVE
#define OCW1_SERI2	0x8	// reserved on SLAVE
#define OCW1_SERI1	0x10
#define OCW1_MOUSE	(OCW1_SERI1 << 8)
#define OCW1_FDISK	0x20
#define OCW1_FPU	(OCW1_FDISK << 8)
#define OCW1_FLPPY	0x40
#define OCW1_PRIHD	(OCW1_FLPPY << 8)
#define OCW1_SECHD	0x80
#define OCW1_PPRNT	(OCW1_SECHD << 8)
#define OCW1_INTRS_OFF	0xFF

// OCW3 Constants
#define OCW3_READ_IRR	0xA
#define OCW3_READ_ISR	0xB


#define PIC_EOI		0x20

// function definition

void i8259_init(void);
void i8259_disable(void);
void i8259_mask_irq(uint16_t);
void i8259_unmask_irq(uint16_t);
uint16_t i8259_read_isr(void);
uint16_t i8259_read_irr(void);
void i8259_eoi(uint8_t eoi);
#endif
