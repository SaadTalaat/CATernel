/**
 * @file apic.h
 * @author Saad Talaat , Menna Essa
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

/*ICR Delivery_mode mode values , defines the type of IPI to be sent*/
#define ICR_FIXED 		0x0
#define ICR_LO_PIR 		0X1 	/*lowest piriority*/
#define ICR_SMI		0x2 	/*SMI Interrupt*/
#define ICR_NMI		0x4	/*NMI interrupt*/
#define ICR_INIT		0x5     /*INIT REQ*/
#define ICR_SIPI		0x6     /*Startup*/
/*ICR Destination modes*/
#define PHYSICAL	0x0
#define LOGICAL 	0x1
/*ICR Delivery stats*/
#define ICR_IDLE 		0x0
#define ICR_PENDING 	0x1
/** Levels. */
#define LEVEL_DEASSERT  0x0
#define LEVEL_ASSERT    0x1
/** Trigger Modes. */
#define TRIGMOD_EDGE   0x0
#define TRIGMOD_LEVEL  0x1
/*ICR Destination short hand */
#define NO_SH		 0x0 	/*no short hand*/
#define SELF		 0x1 	/*send to self*/
#define ALL_W_SELF	 0x2 	/*all with self*/
#define ALL_X_SELF	 0x3    /*all excluding self*/

#define LAVR_Mask  0xffU
#define is_82489DX_apic(x)  ((((x) & LAVR_Mask & 0xf0U) == 0x0U))

//refer to 10.6.2 intel manual
typedef struct {
	union {
		uint32_t lo;
		struct {
			uint8_t vector;                		 /*Interrupt Vector*/
			unsigned int delivery_mode : 3;          /*Delivery Mode*/
			unsigned int dest_mode : 1;       	 /*Destination Mode*/
			unsigned int delivery_stat : 1;          /*Delivery status*/
			unsigned int : 1;               	 /*Reserved*/
			unsigned int level : 1;        		 /*Level*/
			unsigned int trigger_mode : 1;  	 /*Trigger Mode*/
			unsigned int : 2;               	 /*Reserved*/
			unsigned int shorthand : 2;     	 /*Destination Shorthand*/
			unsigned int : 12;              	 /*Reserved*/
		} __attribute__ ((packed));
	};
	union {
		uint32_t hi;
		struct {
			unsigned int : 24;  /*Reserved*/
			uint8_t dest;       /*Destination field*/
		} __attribute__ ((packed));
	};
} __attribute__ ((packed)) icr_t;

/* Function headers */
void soft_lapic_enable(void);
void msr_lapic_enable(void);
void apic_init_ipi(uint8_t lapicid);
void apic_s_ipi(icr_t icr , uint8_t lapicid);
void apic_read_errors(void);
void vector_test(void);
#endif /* _CATERNEL_X86_MP_APIC_H_ */
