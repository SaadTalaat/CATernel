/**
 * @addtogroup Synchronization
 * @{
 * @file waitq.h
 * @brief Wait queue structures and headers
 * @author Saad Talaat
 * @date 1st December, 2012
 */

#ifndef _CATERNEL_SYNCH_WAITQ_H_
#define _CATERNEL_SYNCH_WAITQ_H_
#include <proc/proc.h>
typedef struct waiting_proc{
	LIST_ENTRY(waiting_proc)	next;
	proc_t	*proc;
} waiting_t;

LIST_HEAD(Wait_list, waiting_proc);
//extern struct Wait_queue waiting_procs;
void wait_init(struct Proc_List *);
void wait_sleep(struct Proc_List *, proc_t *,  uint32_t);
void wait_update(void) __attribute__((unused));
void wait_wakeup(struct Proc_List *list);


#endif
/**
 * @}
 */
