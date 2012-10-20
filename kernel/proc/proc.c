#include <types.h>
#include <memvals.h>
#include <proc/proc.h>
#include <arch/x86/mm/segdesc.h>
#include <arch/x86/processor.h>
#include <string.h>
#include <init.h>
#include <rdisk.h>

extern pde_t *global_pgdir;
struct Proc_List	empty_procs;

proc_t *proc_table;
struct Proc_Lifo	running_procs;

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

	/*
	 * Initate all processes slots to empty.
	 */
	for(i_proc = &proc_table[0], count =0 ; count < MAX_PROCS; i_proc++, count++){

		i_proc->gpr_regs = (gpr_regs_t){0,0,0,0,0,0,0,0};
		i_proc->seg_regs = (seg_regs_t){0,0,0,0};
		i_proc->proc_status = PROC_EMPTY;
		i_proc->proc_id	= count;
		i_proc->preempted = 0;
		i_proc->dequeqed = 0;
		i_proc->eip	= 0;
		i_proc->cs	= 0;
		i_proc->ss	= 0;
		i_proc->esp	= 0;
		i_proc->eflags	= 0;
		
		LIST_INSERT_HEAD(&empty_procs, &proc_table[count], link);
	}
	// Setup user space 16 Megs are enough..
	map_segment_page(global_pgdir, 0xA0000000, 0x1000000 ,
			0x20000000 , PAGE_PRESENT | PAGE_USER | PAGE_WRITABLE);


}


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

	proc->esp	=	USERSTACK_TOP - 50;
	proc->cs	=	SEG_USERCODE | 3;
	proc->ss	=	SEG_USERDATA | 3;
	proc->proc_id	= proc- proc_table;
	LIST_REMOVE(proc,link);
	*proc_s = proc;
	return 0;
}
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
uint32_t
init_proc0()
{
	proc_t *proc0;
	create_proc(&proc0);
	elf_load_to_proc(proc0, 512*127);
	//write_cr3(proc0->cr3);
	//switch_address_space(proc0);

	/* Not reachable */
}
void
init_proc(void)
{
	init_proc_table();
	test_lifo();
	init_proc0();
}
void
test_lifo(void){
	proc_t *proc_out;
	LIFO_PUSH(&running_procs, &proc_table[0], q_link);
	LIFO_PUSH(&running_procs, &proc_table[1], q_link);
	LIFO_PUSH(&running_procs, &proc_table[2], q_link);
	LIFO_PUSH(&running_procs, &proc_table[3], q_link);
	proc_out = LIFO_POP(&running_procs, q_link);
	assert(proc_out->proc_id == 3);
	proc_out = LIFO_POP(&running_procs, q_link);
	assert(proc_out->proc_id == 2);
	proc_out = LIFO_POP(&running_procs, q_link);
	assert(proc_out->proc_id == 1);
	proc_out = LIFO_POP(&running_procs, q_link);
	assert(proc_out->proc_id == 0);
	printk("[*] TEST: LIFO Queue test passed..\n");
	return;

}
void
switch_address_space(proc_t *proc_to_run){
	/*
	 * Fooling the x86 CPU!
	 *      1- set stack top to point at proc structure start
	 *      2- set the cpu state to what the proc indicates
	 *      3- issue an iret
	 */
	proc_to_run->seg_regs.fs = 0x23;
	proc_to_run->seg_regs.es = 0x23;
	proc_to_run->seg_regs.gs = 0x23;
	proc_to_run->seg_regs.ds = 0x23;
	asm("xchg %bx,%bx");
//	asm volatile("cli");
	LIFO_PUSH(&running_procs, proc_to_run, q_link);
	asm volatile("movl %0,%%esp":: "g" (proc_to_run) : "memory");

	asm volatile("popal");
	asm volatile("popl %gs\npopl %fs\npopl %es");
	asm volatile("popl %ds");
//	asm volatile("movl $5f,0(%esp)\n");
	asm volatile("iret\n5:\n");

	while(1);
}

