#include <structs/linkedlist.h>
#include <synchronization/wait_queue.h>
#include <arch/x86/mm/page.h>

struct Proc_List waiting_procs;
//waiting_t *empty_waiting;

void
waitq_init(void){
	LIST_INIT(&waiting_procs);
//	empty_waiting = allocate(PAGESZ*sizeof(waiting_t), PAGESZ);
}
void
wait_sleep(proc_t *proc,  uint32_t ticks)
{
	LIST_INSERT_HEAD(&waiting_procs, proc, link);
	proc->timer = ticks;
}

void
wait_wakeup(void)
{
	proc_t *proc;
	LIST_FOREACH(proc, &waiting_procs, link)
	{
		proc->timer --;
		if(proc->timer == 0)
		{
			LIST_REMOVE(proc,link);
			proc_ready(proc);
		}
	}

}
