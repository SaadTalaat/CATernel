/**
 * @addtogroup Synchronization
 * @{
 * @file wait_queue.c
 * @author Saad Talaat
 * @name Wait queues
 * @brief Wait queues
 */

#include <structs/linkedlist.h>
#include <synchronization/wait_queue.h>
#include <arch/x86/mm/page.h>

/**
 * @param waiting_procs , list to hold waiting procs pointer
 * @return nothing
 * @brief Waiting procs linked list initialization function
 */
void
wait_init(struct Proc_List *waiting_procs){
	LIST_INIT(waiting_procs);
//	empty_waiting = allocate(PAGESZ*sizeof(waiting_t), PAGESZ);
}

/**
 * @param list, the sleeping processes list
 * @param proc, the process to put to sleep
 * @param ticks, number of ticks before wake-up
 * @return void
 * @brief function that blocks a process
 * @details
 * Generally, a sleep is done on an I/O or race conditions
 * Here, a sleep is done either timed out or forever..relatively.
 * and proc status is changed into blocked, inserted into waiting
 * list and scheduler is set to reschedule procs.
 */
void
wait_sleep(struct Proc_List *list, proc_t *proc,  uint32_t ticks)
{
	if(list == NULL)
	{
		panic("Bad waiting queue!!\n");
	}
	if(ticks != 0){
		proc->timer = ticks;
	}
	else
		//that's a freaking long time, zombies and deadlocks to be handled later..
		proc->timer = 0x3FFFFFFF;
	proc->status = BLOCKED;
	LIST_INSERT_HEAD(list, proc, wait_link);
	schedule();
}

/**
 * @param nothing
 * @return nothing
 * @brief blocked process updating function
 * @details
 * For each timer tick/interrupt a wait_update is made
 * to wake up timed out procs and update other waiting
 * procs timers.
 * __NOT_USED__
 */
void
wait_update(void)
{
	/*proc_t *proc;
	LIST_FOREACH(proc, &waiting_procs, wait_link)
	{
		proc->timer --;
		if(proc->timer == 0)
		{
			LIST_REMOVE(proc,wait_link);
			proc->status = BLOCKED;
			proc_ready(proc);
		}
	}*/
	return;

}

/**
 * @param list , the blocked processes list
 * @return nothing
 * @brief wakes up a proccess once timeout or resource free
 * @details
 * Basically, This function serves procs blocked due
 * to a lock or semaphore..etc
 * the first proc is marked as ready and pushed into ready procs
 */
void
wait_wakeup(struct Proc_List *list)
{

	proc_t *blocked;
	blocked = LIST_FIRST(list);
	blocked->status = RUNNABLE;
	LIST_REMOVE(blocked, wait_link);
	proc_ready(blocked);
	// I shall not schedule immediately
/*
	proc_t *proc, *nproc;
	LIST_FOREACH(proc, &waiting_procs, wait_link)
	{
		if(proc->blocking_id == bproc->id)
		{
			nproc = proc;
			proc->blocking_id = 0;
			proc->status = RUNNABLE;
			proc_ready(proc);
			break; // sorry fellas, only one at a time..
		}
	}
	LIST_FOREACH(proc, &waiting_procs, wait_link)
	{
		if(proc->blocking_id == bproc->id)
		{
			proc->blocking_id = nproc->id;
		}
	}
	return;
*/
}


/**
 * @}
 */
