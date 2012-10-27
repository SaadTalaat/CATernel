/**
 * @file proc.h
 * @author Saad Talaat
 * @date 4 Oct, 2012
 * @brief Process manager header file
 */


#ifndef _CATERNEL_PROC_H_
#define _CATERNEL_PROC_H_
#include <arch/x86/cpu_state.h>
#include <memvals.h>
#include <structs/queue.h>
#include <arch/x86/mm/page.h>

#define MAX_PROCS	256
#define MAX_PROC_NAME	128

#define RUNNABLE	0
#define PROC_EMPTY	0x1
#define NON_RUNNABLE	0x2
typedef struct proc {
	gpr_regs_t 	gpr_regs;
	seg_regs_t 	seg_regs;
	reg_t		eip;
	uint32_t		cs;
	reg_t		eflags;
	reg_t		esp;
	uint32_t		ss;
	uint32_t	proc_id;
	uint32_t	proc_status;
	
	pde_t		*page_directory;
	uint32_t	cr3;
	uint32_t	preempted;
	uint32_t	dequeqed;
//	char 		proc_name[MAX_PROC_NAME];
	LIST_ENTRY(proc)	link;
	LIFO_ENTRY(proc)	q_link;
} proc_t;

#define PROC_TABLE_SIZE	ROUND_UP(MAX_PROCS * sizeof(struct proc), PAGESZ)

/*
 * Define a Proc_List structure to hold empty procs
 *
 */
LIST_HEAD(Proc_List, proc);
LIFO_HEAD(Proc_Lifo, proc);
/* Process Table */
extern proc_t *proc_table;
extern struct Proc_Lifo running_procs;

void init_proc_table(void);
void switch_address_space(proc_t*);
uint32_t proc_setup(proc_t **);
uint32_t proc_setup_mem(proc_t *);
void test_lifo(void);
#endif
