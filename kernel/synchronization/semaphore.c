#include <synchronization/semaphore.h>

void
semaphore_init(semaphore_t *s, uint32_t num)
{
	s->count = num;
	wait_init(&(s->wait_list));
//	LIST_INIT(&(s->wait_list));
}
void
semaphore_down(semaphore_t *s, proc_t* proc)
{
	/**
	 * aquire semaphore
	 * If a resource is available aquire it
	 * else wait with max time till waken up on resource release
	 */
	if(s->count > 0)
		s->count--;
	else
		wait_sleep(&(s->wait_list), proc, 0);
}

void
semaphore_up(semaphore_t *s)
{
	/**
	 * release semaphore
	 *
	 */
	proc_t *waiting_proc;
	if(LIST_EMPTY(&(s->wait_list)))
		s->count ++;
	else
		wait_wakeup(&(s->wait_list));
}
