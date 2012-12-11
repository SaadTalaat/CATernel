#ifndef _CATERNEL_SYNCH_SEMAPHORE_H_
#define _CATERNEL_SYNCH_SEMAPHORE_H_

#include <synchronization/wait_queue.h>
typedef struct{
	uint32_t count;
	struct Proc_List wait_list;
} semaphore_t;

void semaphore_init(semaphore_t *, uint32_t);
void semaphore_down(semaphore_t *, proc_t*);
void semaphore_up(semaphore_t *s);
#endif
