#include <synchronization/semaphore.h>

void
mutex_init(semaphore_t *s){
	s->count = 1;
	wait_init(&(s->wait_list));
}

void
mutex_lock(semaphore_t *s, proc_t *proc){
	if(s->count == 1)
		s->count =0;
	else
		wait_sleep(&(s->wait_list), proc, 0);
}

void
mutex_unlock(semaphore_t *s){
        proc_t *waiting_proc;
        if(LIST_EMPTY(&(s->wait_list)))
                s->count ++;
        else
                wait_wakeup(&(s->wait_list));

}
