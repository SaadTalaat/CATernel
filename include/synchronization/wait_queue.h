/**
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

//LIST_HEAD(Wait_queue,);
//extern struct Wait_queue waiting_procs;
#endif
