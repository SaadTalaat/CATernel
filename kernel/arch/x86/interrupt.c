/**
 * @addtogroup Interrupts-and-Syscalls
 * @{
 * @file interrupt.c
 * @author Saad Talaat
 * @date 27 Sept, 2012
 * @brief Interrupt and fault handling
 * @name Interrupts Initialization
 * @{
 */
#include <types.h>
#include <test.h>
#include <proc/proc.h>
#include <arch/x86/x86.h>
#include <arch/x86/interrupt.h>
#include <arch/x86/vectors.h>
#include <arch/x86/mm/segdesc.h>
#include <arch/x86/cpu_state.h>
#include <syscalls/syscalls.h>
#include <drivers/i8259.h>
/**
 * @brief Declare IDT with 255 width interrupts.
 */
gatedesc idt[64];

/**
 * @brief Fault names.
 */
char *x86_exception_names[] ={
	"Divide Error #DE",
	"Debug",
	"NMI",
	"Breakpoint #BP",
	"Overflow #OV",
	"Bound Range Exceeded #BR",
	"Undefined Opcode",
	"Device not avalible",
	"Double fault",
	"Coprocessor segment overrun",
	"Invalid TSS",
	"Segment not present",
	"Stack Segment Fault",
	"General Protection",
	"Page Fault",
	"\0",
	"x87 FPU error",
	"Alignment Mask",
	"Machine Check",
	"SIMD exception"
	};
extern int_generic;

void
trap(void){
	printk("[*] Trap issued \n");
	return;
}

/**
 * @brief Initializing Interrupt Descriptor table
 * @details
 * the idt init fills the idt table with generic
 * interrupt vector which actually does nothing.
 * later on interrupt initialization we'll set
 * specific interrupt numbers to specific vectors
 * [vector_X] is independent proc, that does
 * rely on interrupt number, which is a drawback
 * indeed but it's fixable later on.
 *
 */
void
idt_init(void){
	gatedesc *gate;
	
	uint32_t count, offset;	
	
	for(count =0; count < IDT_ENTRIES; count++){
	//offset = (uint32_t) &vector_0;
	gate = &idt[count];
	//gate->offset_0_15 = (uint16_t) (offset & 0xFFFF);
	gate->segment_s	= SEG_KERNCODE;
	gate->args = gate->reserved = 0;
	gate->type = GATE_INT32;
	gate->s	= 0;
	gate->dpl = 0;
	if(count == SYSCALL || count == SCHED || count == PF)
		gate->dpl = 3;
	gate->p	= 1;
	//gate->offset_16_31 = (uint16_t) (offset >> 16);

//	for some reason GATE_FILL wont work :@
//	GATE_FILL(&idt[count], GATE_INT32, 0, SEG_KERNCODE, ((uint32_t) &int_generic));

	}
	GATE_OFFSET(idt[0],&vector_0);
	GATE_OFFSET(idt[1],&vector_1);
	GATE_OFFSET(idt[2],&vector_2);
	GATE_OFFSET(idt[3],&vector_3);
	GATE_OFFSET(idt[4],&vector_4);
	GATE_OFFSET(idt[5],&vector_5);
	GATE_OFFSET(idt[6],&vector_6);
	GATE_OFFSET(idt[7],&vector_7);
	GATE_OFFSET(idt[8],&vector_8);
	GATE_OFFSET(idt[9],&vector_9);
	GATE_OFFSET(idt[10],&vector_10);
	GATE_OFFSET(idt[11],&vector_11);
	GATE_OFFSET(idt[12],&vector_12);
	GATE_OFFSET(idt[13],&vector_13);
	GATE_OFFSET(idt[14],&vector_14);
	GATE_OFFSET(idt[15],&vector_15);
	GATE_OFFSET(idt[16],&vector_16);
	GATE_OFFSET(idt[17],&vector_17);
	GATE_OFFSET(idt[18],&vector_18);
	GATE_OFFSET(idt[19],&vector_19);
	GATE_OFFSET(idt[20],&vector_20);
	GATE_OFFSET(idt[21],&vector_21);
	GATE_OFFSET(idt[22],&vector_22);
	GATE_OFFSET(idt[23],&vector_23);
	GATE_OFFSET(idt[24],&vector_24);
	GATE_OFFSET(idt[25],&vector_25);
	GATE_OFFSET(idt[26],&vector_26);
	GATE_OFFSET(idt[27],&vector_27);
	GATE_OFFSET(idt[28],&vector_28);
	GATE_OFFSET(idt[29],&vector_29);
	GATE_OFFSET(idt[30],&vector_30);
	GATE_OFFSET(idt[31],&vector_31);
	GATE_OFFSET(idt[32],&vector_32);
	GATE_OFFSET(idt[33],&vector_33);
	GATE_OFFSET(idt[34],&vector_34);
	GATE_OFFSET(idt[35],&vector_35);
	GATE_OFFSET(idt[36],&vector_36);
	GATE_OFFSET(idt[37],&vector_37);
	GATE_OFFSET(idt[38],&vector_38);
	GATE_OFFSET(idt[39],&vector_39);
	GATE_OFFSET(idt[40],&vector_40);
	GATE_OFFSET(idt[41],&vector_41);
	GATE_OFFSET(idt[42],&vector_42);
	GATE_OFFSET(idt[43],&vector_43);
	GATE_OFFSET(idt[44],&vector_44);
	GATE_OFFSET(idt[45],&vector_45);
	GATE_OFFSET(idt[46],&vector_46);
	GATE_OFFSET(idt[47],&vector_47);
	GATE_OFFSET(idt[48],&vector_48);
	GATE_OFFSET(idt[49],&vector_49);
	GATE_OFFSET(idt[50],&vector_50);
	GATE_OFFSET(idt[51],&vector_51);
	GATE_OFFSET(idt[52],&vector_52);
	GATE_OFFSET(idt[53],&vector_53);
	GATE_OFFSET(idt[54],&vector_54);
	GATE_OFFSET(idt[55],&vector_55);
	GATE_OFFSET(idt[56],&vector_56);
	GATE_OFFSET(idt[57],&vector_57);
	GATE_OFFSET(idt[58],&vector_58);
	GATE_OFFSET(idt[59],&vector_59);
	GATE_OFFSET(idt[60],&vector_60);
	GATE_OFFSET(idt[61],&vector_61);
	GATE_OFFSET(idt[62],&vector_62);
	GATE_OFFSET(idt[63],&vector_63);
	printk("[*] Interrupt descriptor table initialized\n\tIrqs Lower than 63 ignored.\n");
	for(count = 64; count<256; count++)
	{
		GATE_OFFSET(idt[count],&int_generic);
	}

}

/**
 * @param index of the interrupt OR interrupt offset in IDT
 * @param *name the name to be attached to interrupt.
 * @param either interrupt is present or not.
 * @param interrupt handler.
 * @brief register an interrupt handling procedure
 */
void
register_exception(uint32_t index, char *name, uint16_t present, void (handler)(void)){

	return;
}

void
interrupt_init(void){
	return;

}

/**
 * @brief maps an exception/interrupt to it's handler
 * @details
 * This function is called by the vector_x procedure and
 * used to map faults to handler function like page fault
 * handler and system call handler and zero division handler..etc.
 */
void
map_exception(uint32_t int_index, cpu_state_t *cpu_state){
#ifdef	_SYSDBG_
	printk("================= Exception =================\n");
	printk("%s of index %d\n", x86_exception_names[int_index], int_index);
	printk("Error Code = %x\n", cpu_state->error_code);
	printk("============ Current CPU State ==============\n");
	printk("Segment Registers :-\n");
	printk("\tCS=%p, DS=%p\n",cpu_state->cs , cpu_state->ds );
	printk("\tES=%p, FS=%p\n",cpu_state->es , cpu_state->fs);
	printk("\tGS=%p, SS=%p\n", cpu_state->gs,cpu_state->ss);
	printk("GPR Regisers :-\n");
	printk("\tEAX=%p, EBX=%p, ECX=%p, EDX=%p\n",
			cpu_state->eax, cpu_state->ebx, cpu_state->ecx, cpu_state->edx);
	printk("\tEDI=%p, ESI=%p, EBP=%p, ESP=%p\n",
			cpu_state->edi, cpu_state->esi, cpu_state->o_ebp, cpu_state->esp);
	printk("\tEFLAGS = %p\n", cpu_state->eflags);
	printk("Current Instruction:-\n");
	printk("\tEIP=%p\n",cpu_state->eip);
#endif
	printk("[*] INT : %d\n", int_index);
	switch(int_index)
	{
		case PF:
			page_fault_handler(cpu_state);
			break;
		case SYSCALL:
			map_syscall(cpu_state);
			break;
		case SCHED:
			schedule();
			break;
		default:
			return;
	}
/*	if(int_index == PF)
		page_fault_handler(cpu_state);
	if(int_index == SYSCALL)
		map_syscall(cpu_state);
	if((int_index >= 40) && (int_index < 48))
	{
		asm("xchg %bx,%bx");
		i8259_eoi(int_index-32);
	}
*/	
	return;
}

/**
 * @brief Page Fault Handler
 * @details Page fault handler \n
 *  Check if proc is accessing kernel space and not kernel mode. \n
 * If from kernel, The kernel panics. if from user space, stack is extended
 * if needed or proc is killed.
 * 
 */
uint32_t
page_fault_handler(cpu_state_t *cpu_state)
{
	
	uint32_t page_fault_src, offby, extension_max = 4;
	/* What address caused page fault? */
	page_fault_src = read_cr2();
	
	if( (page_fault_src & 0xF0000000) == 0xF0000000)
	{
		/* did it come from kernel mode? */
		if(cpu_state->error_code & PF_FROM_KERNEL)
		{
			panic("Page fault from kernel space, from addr %p", page_fault_src);
		}
		/* did it come from user code? and ofc it did..*/
		else
		{
			/* kill causing proc */
			printk("Page Fault\n");
			LIFO_POP(&running_procs,q_link);
		}
	}
	/* If it is caused by instruction fetch from user */
	if((cpu_state->error_code & PF_IFETCH) && (cpu_state->error_code & PF_FROM_USER))
	{
		printk("Page Fault\n");
		LIFO_POP(&running_procs, q_link);
	}
	else if (cpu_state->error_code & PF_FROM_USER)
	{
		if( (offby = (((page_fault_src & ~0xFFF) - USERSTACK_TOP) % PAGESZ )) == 0
				&& offby <= extension_max )
			/* allocate a page for it */
		{
			struct Page *page;
			proc_t *current_proc;
			x86_page_alloc(&page);
			current_proc = LIFO_POP(&running_procs,q_link);
			x86_page_insert(current_proc->page_directory, page, (void *) (page_fault_src & ~0xFFF),
				PAGE_PRESENT | PAGE_USER);
			LIFO_PUSH(&running_procs, current_proc, q_link);
			return 0;
		}

	}
}

/**
 * @}
 * @}
 */
