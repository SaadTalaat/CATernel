/** include/arch/x86/interrupt.h
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 *
 * @date 27 Sept, 2012
 *
 */
#include <arch/x86/interrupt.h>
#include <arch/x86/vectors.h>
#include <arch/x86/mm/segdesc.h>
#include <arch/x86/cpu_state.h>
/*
 * Declare our IDT with 255 width interrupts.
 */
gatedesc idt[256];

extern int_generic;
void
trap(void){
	printk("[*] Trap issued \n");
	return;
}

/*
 * the idt init fills the idt table with generic
 * interrupt vector which actually does nothing.
 * later on interrupt initialization we'll set
 * specific interrupt numbers to specific vectors
 * int_generic is independent proc, that doesn't
 * rely on interrupt number, which is a drawback
 * indeed but it's fixable later on.
 *
 */
void
idt_init(void){
	gatedesc *gate;
	
	uint32_t count, offset;	
	
	for(count =0; count < IDT_ENTRIES; count++){
//	offset = (uint32_t) &(VECTOR_INDEX(count));
	gate = &idt[count];
//	gate->offset_0_15 = (uint16_t) (offset & 0xFFFF);
	gate->segment_s	= SEG_KERNCODE;
	gate->args = gate->reserved = 0;
	gate->type = GATE_INT32;
	gate->s	= 0;
	gate->dpl = 0;
	gate->p	= 1;
//	gate->offset_16_31 = (uint16_t) (offset >> 16);

//	for some reason GATE_FILL wont work :@
//	GATE_FILL(&idt[count], GATE_INT32, 0, SEG_KERNCODE, ((uint32_t) &int_generic));

	}
	GATE_OFFSET(idt[0],vector_0);
	GATE_OFFSET(idt[1],vector_1);
	GATE_OFFSET(idt[2],vector_2);
	GATE_OFFSET(idt[3],vector_3);
	GATE_OFFSET(idt[4],vector_4);
	GATE_OFFSET(idt[5],vector_5);
	GATE_OFFSET(idt[6],vector_6);
	GATE_OFFSET(idt[7],vector_7);
	GATE_OFFSET(idt[8],vector_8);
	GATE_OFFSET(idt[9],vector_9);
	GATE_OFFSET(idt[10],vector_10);
	GATE_OFFSET(idt[11],vector_11);
	GATE_OFFSET(idt[12],vector_12);
	GATE_OFFSET(idt[13],vector_13);
	GATE_OFFSET(idt[14],vector_14);
	GATE_OFFSET(idt[15],vector_15);
	GATE_OFFSET(idt[16],vector_16);
	GATE_OFFSET(idt[17],vector_17);
	GATE_OFFSET(idt[18],vector_18);
	GATE_OFFSET(idt[19],vector_19);
	GATE_OFFSET(idt[20],vector_20);
	GATE_OFFSET(idt[21],vector_21);
	GATE_OFFSET(idt[22],vector_22);
	GATE_OFFSET(idt[23],vector_23);
	GATE_OFFSET(idt[24],vector_24);
	GATE_OFFSET(idt[25],vector_25);
	GATE_OFFSET(idt[26],vector_26);
	GATE_OFFSET(idt[27],vector_27);
	GATE_OFFSET(idt[28],vector_28);
	GATE_OFFSET(idt[29],vector_29);
	GATE_OFFSET(idt[30],vector_30);
	GATE_OFFSET(idt[31],vector_31);
	GATE_OFFSET(idt[32],vector_32);
	GATE_OFFSET(idt[33],vector_33);
	GATE_OFFSET(idt[34],vector_34);
	GATE_OFFSET(idt[35],vector_35);
	GATE_OFFSET(idt[36],vector_36);
	GATE_OFFSET(idt[37],vector_37);
	GATE_OFFSET(idt[38],vector_38);
	GATE_OFFSET(idt[39],vector_39);
	GATE_OFFSET(idt[40],vector_40);
	GATE_OFFSET(idt[41],vector_41);
	GATE_OFFSET(idt[42],vector_42);
	GATE_OFFSET(idt[43],vector_43);
	GATE_OFFSET(idt[44],vector_44);
	GATE_OFFSET(idt[45],vector_45);
	GATE_OFFSET(idt[46],vector_46);
	GATE_OFFSET(idt[47],vector_47);
	GATE_OFFSET(idt[48],vector_48);
	GATE_OFFSET(idt[49],vector_49);
	GATE_OFFSET(idt[50],vector_50);
	GATE_OFFSET(idt[51],vector_51);
	GATE_OFFSET(idt[52],vector_52);
	GATE_OFFSET(idt[53],vector_53);
	GATE_OFFSET(idt[54],vector_54);
	GATE_OFFSET(idt[55],vector_55);
	GATE_OFFSET(idt[56],vector_56);
	GATE_OFFSET(idt[57],vector_57);
	GATE_OFFSET(idt[58],vector_58);
	GATE_OFFSET(idt[59],vector_59);
	GATE_OFFSET(idt[60],vector_60);
	GATE_OFFSET(idt[61],vector_61);
	GATE_OFFSET(idt[62],vector_62);
	GATE_OFFSET(idt[63],vector_63);


}

void
register_exception(uint32_t index, char *name, uint16_t present, void (handler)(void)){

	return;
}

void
interrupt_init(void){
	return;

}

void
map_exception(cpu_state_t cpu_state, int int_index){

}
