/**
 * @file apic.h
 * @author Saad Talaat
 * @brief Support for xAPIC for MPs
 * @date 18 November, 2012
 *
 */

#ifndef _CATERNEL_X86_MP_APIC_H_
#define _CATERNEL_X86_MP_APIC_H_
#include <types.h>
extern uint32_t *lapic;

/* APIC Index in MSR */
#define IA32_APIC_BASE	0x1B
#define IA32_APIC_BASE_BSP	0x8
#define IA32_APIC_BASE_ENABLE	0xB
#define IA32_APIC_BASE_ADDR	0xC

/* Local APIC indecies */
#define LAID	0x8	/* RW - Local APIC ID */
#define LAVR	0x12	/* R  - Local APIC Version */
#define TPR	0x20	/* RW - Task Priority Register */
#define APR	0x24	/* R  - Arbitration Priority Register */
#define PPR	0x28	/* R  - Processor Priority Register */
#define EOI	0x2C	/* W  - EOI Register */
#define RRD	0x30	/* R  - Remote Read Register */
#define LDR	0x34	/* RW - Logical destination register */
#define DFR	0x38	/* RW - Destination format register */
#define SVR	0x3C	/* RW - Spurious interrupts */
#define ERR	0xA0	/* R  - Error status register */
#define ICR_LOW	0xC0	/* RW - ICR 0~31 */
#define ICR_HIGH 0xC4	/* RW - ICR 32~63 */
#define ICT	0xE0	/* RW - Initial Count for timer */
#define CCT	0xE4	/* R  - Current count for timer */
#define DCT	0xF8	/* RW - Divide configuration for timer */

/* LVT Indecies */
#define LVT_CMCI	0xBC
#define LVT_TIMER	0xC8	/* RW - APIC Timer interrupt register */
#define LVT_TMR		0xCC	/* RW - APIC Thermal monitor register */
#define LVT_PCR		0xD0	/* RW - APIC performance counter register */
#define LVT_LINT0	0xD4	/* RW - APIC LINT0 */
#define LVT_LINT1	0xD8	/* RW - APIC LINT1 */
#define LVT_ERR		0xDC	/* RW - APIC Error register */

/* Function headers */
void soft_lapic_enable(void);
void msr_lapic_enable(void);
#endif /* _CATERNEL_X86_MP_APIC_H_ */
