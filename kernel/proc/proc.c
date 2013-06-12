/**
odefine FIFO_EXTERN(nme, member)\
	extern struct fifo name;
 * @addtogroup Process-Management
 * @{
 * @file proc.c
 * @author Saad Talaat
 * @date Tuesday 16/10/2012
 * @brief Process Manager.
 * @name Process Manager.
 * @{
 *
 */
#include <types.h>
#include <memvals.h>
#include <proc/proc.h>
#include <arch/x86/mm/segdesc.h>
#include <arch/x86/processor.h>
#include <string.h>
#include <init.h>
#include <rdisk.h>
#include <drivers/i8254.h>

extern pde_t *global_pgdir;
struct Proc_List	empty_procs;

proc_t *proc_table;
struct Proc_Lifo	running_procs;
FIFO_HEAD(ready_procs, proc,256);
/**
 * @brief Initialize the proc array
 * @details
 * Initializes all the procs data structures, Proc table, empty proc list
 * and the scheduling process queues.
 * All proc entries are set to 0's. and empty proc list is fill. and
 * proc binary loader is mapped.
 */
void
proc_printinfo(void)
{
	printk("[*] Proc Info:\n\t");
	printk("Max Processes : %d\n\t", MAX_PROCS);
	printk("Free Table    : %p\n\t", &empty_procs);
	printk("Running Queue : %p\n\t",&running_procs);
	printk("Ready Queue   : %p\n" , &ready_procs);
	return;
}

/**
 * @param nothing
 * @return nothing
 * @details
 * Commits process holding structures (LIST/LIFO/FIFO)
 * initalizes PEBs and address spaces.
 */
void
init_proc_table(void){
	/*
	 * In Memory Map, Processes Table takes places after
	 * the pages table. at PROC_LIST
	 */

	int 	c,count;
	proc_t	*i_proc;
	LIST_INIT(&empty_procs);
	LIFO_INIT(&running_procs);
	FIFO_INIT(&ready_procs);

	/*
	 * Initate all processes slots to empty.
	 */
	for(i_proc = &proc_table[0], count =0 ; count < MAX_PROCS; i_proc++, count++){

		i_proc->gpr_regs = (gpr_regs_t){0,0,0,0,0,0,0,0};
		i_proc->seg_regs = (seg_regs_t){0,0,0,0};
		i_proc->status = PROC_EMPTY;
		i_proc->id	= count;
		i_proc->preempted = 0;
		i_proc->dequeqed = 0;
		i_proc->eip	= 0;
		i_proc->cs	= 0;
		i_proc->ss	= 0;
		i_proc->esp	= 0;
		i_proc->eflags	= 0 | FLAG_IF;
		i_proc->timer 	= 0;
		LIST_INSERT_HEAD(&empty_procs, &proc_table[count], link);
	}
	// Setup user space 16 Megs are enough..
	map_segment_page(global_pgdir, 0xA0000000, 0x1000000 ,
			0x20000000 , PAGE_PRESENT | PAGE_USER | PAGE_WRITABLE);


}

/**
 * @param proc_t proc pointer reference to store created proc in.
 * @return 0 if success
 * @brief Initialize and create a proc information block.
 * @details a proc address space is initialized and
 * segment desciptors are set, and the Stack is also set.
 */
uint32_t
create_proc(proc_t **proc_s)
{
	proc_t *proc;
	struct Page *page;
	int count;
	proc = LIST_FIRST(&empty_procs);
	x86_page_alloc(&page);
	pde_t *pgdir = pagetova(page);
	memset(pgdir, 0, PAGESZ);
	proc->cr3 = KA2PA(pgdir);
	proc->page_directory = pagetova(page);
	
	// Copy vas from current page dir
	for(count=PGDIRX(USERSTART);count < 1024; count++)
		proc->page_directory[count] = global_pgdir[count];

	proc->page_directory[PGDIRX(VIRTPGT)] = proc->cr3 | PAGE_PRESENT | PAGE_WRITABLE;
	proc->page_directory[PGDIRX(USERVIRTPGT)] = proc->cr3 | PAGE_PRESENT | PAGE_USER;

	proc->esp	=	USERSTACK_TOP - 100;
	proc->cs	=	SEG_USERCODE | 3;
	proc->ss	=	SEG_USERDATA | 3;
	proc->id	= proc- proc_table;
	LIST_REMOVE(proc,link);
	*proc_s = proc;
	return 0;
}

/**
 * @param proc_t* proc, process to use
 * @return nothing
 * @details
 * this small process is used by scheduler to set
 * a process as ready and push it into ready procs
 * to be scheduled into ready_procs FIFO
 */
void
proc_ready(proc_t *proc)
{
	assert(proc->status == RUNNABLE);
	FIFO_PUSH(&ready_procs, proc);
}
/**
 * @param proc_t proc reference to allocate memory for
 * @param void* the base address of memory needed to be allocated.
 * @param uint32_t  the size of segment.
 * @return 0 if success
 * @brief allocates memory thunk for a proc.
 *
 */
uint32_t
proc_alloc_mem(proc_t *proc, void *va, uint32_t len){
	uint32_t base, thunk;
	struct Page *page;
	
	x86_page_alloc(&page);
	/* Calc base memory */
	base = ROUND_DOWN((uint32_t) va, PAGESZ);
	thunk = ROUND_UP((uint32_t) (len, PGOFF((uint32_t)va)), PAGESZ);

	int count;
	for (count= 0; count <thunk; count+=PAGESZ)
		x86_page_insert(proc->page_directory, page, (void *) base+count,
					PAGE_USER | PAGE_WRITABLE);


}

/**
 * @brief loads and initiated the proc0(first proc)
 * from image.
 */
uint32_t
init_proc0()
{
	proc_t *proc0;
	uint32_t status;
	int i;
	create_proc(&proc0);
	printk("proc is %d\n", proc0->id);
	for( i =0; i< 30; i++)
	{
		status = elf_load_to_proc(proc0, 512*(146+i));
		if(status == 0)
		{
			printk("[*] PROC0: found at sector %d\n", 127+i);
			break;
		}
	}
	printk("PROC FRAME:\n");
	printk("CR3: %x\n", proc0->cr3);
	if(status == -1)
	{
		panic("Couldn't locate PROC0.");
	}
	FIFO_PUSH(&ready_procs, proc0);
	/* Not reachable */
}

/**
 * @brief initiates the process table and tests queues and initates proc0
 */
void
init_proc(void)
{
	init_proc_table();
	test_lifo();
	test_fifo();
	init_proc0();
}

/**
 * @brief Tests the LIFO queues data structure functionality.
 */
void
test_lifo(void){
	proc_t *proc_out;
	LIFO_PUSH(&running_procs, &proc_table[0], q_link);
	LIFO_PUSH(&running_procs, &proc_table[1], q_link);
	LIFO_PUSH(&running_procs, &proc_table[2], q_link);
	LIFO_PUSH(&running_procs, &proc_table[3], q_link);
	proc_out = LIFO_POP(&running_procs, q_link);
	assert(proc_out->id == 3);
	proc_out = LIFO_POP(&running_procs, q_link);
	assert(proc_out->id == 2);
	proc_out = LIFO_POP(&running_procs, q_link);
	assert(proc_out->id == 1);
	proc_out = LIFO_POP(&running_procs, q_link);
	assert(proc_out->id == 0);
	printk("[*] TEST: LIFO Queue test passed..\n");
	return;

}

/**
 * @brief Tests the FIFO queue data structure
 */
void
test_fifo(void){
	proc_t *proc_out;
	FIFO_PUSH(&ready_procs, &proc_table[0]);
	FIFO_PUSH(&ready_procs, &proc_table[1]);
	FIFO_PUSH(&ready_procs, &proc_table[2]);
	FIFO_PUSH(&ready_procs, &proc_table[3]);
	assert(!FIFO_EMPTY(&ready_procs));
	proc_out = FIFO_POP(&ready_procs);
	assert(proc_out->id == 0);
	proc_out = FIFO_POP(&ready_procs);
	assert(proc_out->id == 1);
	proc_out = FIFO_POP(&ready_procs);
	assert(proc_out->id == 2);
	proc_out = FIFO_POP(&ready_procs);
	assert(proc_out->id == 3);
	printk("[*] TEST: FIFO Queue test passed..\n");
	return;
}
/**
 * @brief switches between the kernel and a given proc
 * @details switching to a proc is made from a calling proc
 * which is the kernel to a given proc address space.
 * switching is done by Fooling the x86 into beleiving
 * that it is coming back from an interrupt by setting
 * the hardware interrupt stack frame and issuing an iret
 * switching can also be made by SYSENTER/SYSEXIT.
 */ 
void
switch_address_space(proc_t *proc_to_run){
	/*
	 * Fooling the x86 CPU!
	 *      1- set stack top to point at proc structure start
	 *      2- set the cpu state to what the proc indicates
	 *      3- issue an iret
	 */
	printk("CR3: %x\n", proc_to_run->cr3);
	printk("proc @[%x]\n", proc_to_run);
	proc_to_run->seg_regs.fs = 0x23;
	proc_to_run->seg_regs.es = 0x23;
	proc_to_run->seg_regs.gs = 0x23;
	proc_to_run->seg_regs.ds = 0x23;
//	asm volatile("movl %0,%%esp":: "g" (proc_to_run) : "memory");
	
//	write_cr3(proc_to_run->cr3);
	map_segment_page(proc_to_run->page_directory, (vaddr_t) proc_to_run, sizeof(proc_to_run), (paddr_t) va2pa(global_pgdir,proc_to_run), PAGE_USER| PAGE_PRESENT);
	asm ("xchg %bx,%bx");
	write_cr3(proc_to_run->cr3);
	asm volatile("pushl %0":: "g"(proc_to_run->ss));
	asm volatile("pushl %0":: "g"(proc_to_run->esp));
	asm volatile("pushl %0":: "g"(proc_to_run->eflags));
	asm volatile("pushl %0":: "g"(proc_to_run->cs));
	asm volatile("pushl %0":: "g"(proc_to_run->eip));

	asm volatile("pushl %0":: "g"(proc_to_run->seg_regs.ds));
	asm volatile("pushl %0":: "g"(proc_to_run->seg_regs.es));
	asm volatile("pushl %0":: "g"(proc_to_run->seg_regs.fs));
	asm volatile("pushl %0":: "g"(proc_to_run->seg_regs.gs));
	asm volatile("pushl %0":: "g"(proc_to_run->gpr_regs.eax));
	asm volatile("pushl %0":: "g"(proc_to_run->gpr_regs.ecx));
	asm volatile("pushl %0":: "g"(proc_to_run->gpr_regs.edx));
	asm volatile("pushl %0":: "g"(proc_to_run->gpr_regs.ebx));
	asm volatile("pushl %esp");
	asm volatile("pushl %0":: "g"(proc_to_run->gpr_regs.ebp));
	asm volatile("pushl %0":: "g"(proc_to_run->gpr_regs.esi));
	asm volatile("pushl %0":: "g"(proc_to_run->gpr_regs.edi));
	asm volatile("popal");
	asm volatile("popl %gs\npopl %fs\npopl %es");
	asm volatile("popl %ds");
//	asm volatile("movl %0,%%cr3":: "m"(proc_to_run->cr3));
//	asm volatile("movl $5f,0(%esp)\n");
	asm volatile("iret\n5:\n");

	while(1);
}

void
sched_init(void)
{
	i8254_init();

}

/**
 * @param nothing
 * @return nothing
 * @brief main scheduling function
 * @details
 * a very basic RR scheduling function which detach
 * running procs and push them into ready procs then
 * fetch them.
 */
void
schedule(void)
{
	uint32_t idx= 0;
	proc_t *proc, *pproc;
	write_cr3(global_cr3);
	if(!LIFO_EMPTY(&running_procs))
	{
		pproc = LIFO_POP(&running_procs, q_link);
		printk("[*] Proc running: %d\n",pproc->id);
	}
	else
		printk("[*] No running procs\n");

	if(!FIFO_EMPTY(&ready_procs))
	{
		proc = FIFO_POP(&ready_procs);
		printk("[*] Ready proc: %d\n",proc->id);
	}
	else
	{
		printk("[*] No ready procs found\n");
		proc = pproc;
	}

	printk("Here\n");
	if(!LIFO_EMPTY(&running_procs))
		if(pproc->status == RUNNABLE)
			FIFO_PUSH(&ready_procs, pproc);
	printk("Here\n");
	printk("Procs %p\tProc %p\n", &running_procs, proc);
	LIFO_PUSH(&running_procs, proc ,q_link);
	printk("Here2\n");

	printk("[*] Scheduling to process: %d\n", proc->id);

	switch_address_space(proc);
	/** UNREACHABLE **/
}

/**
 * @} @}
 */
