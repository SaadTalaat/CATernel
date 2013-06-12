/**
 * @addtogroup Synchronization
 * @{
 * @file semaphore.c
 * @author Saad Talaat
 * @name Semaphores
 * @brief Semaphore synchronization primitives
 */

#include <synchronization/semaphore.h>

/**
 * @param s the semaphore to initialize
 * @param num the number of resources (counting/binary )
 * @return nothing
 * @brief Initialization of a semaphore primitive function
 * @details
 * Initialization is simply done by setting semaphore count
 * to given number and initialize the list of waiting procs
 */
void
semaphore_init(semaphore_t *s, uint32_t num)
{
	s->count = num;
	wait_init(&(s->wait_list));
//	LIST_INIT(&(s->wait_list));
}

/**
 * @param s the semaphore to do down for.
 * @param proc the process to block
 * @brief does down operation over semaphore primitive
 */
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

/**
 * @param s the semaphore to free resource for
 * @return nothing
 * @brief Up function for a semaphore.
 * @details
 * Once a resource is free the semaphore counting variable
 * is incrememnted if there's no waiting processes.. else
 * a waiting process is brought out to take its place.
 */
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

/**
 * @}
 */
