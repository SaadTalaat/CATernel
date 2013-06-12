/**
 * @addtogroup Synchronization
 * @{
 * @file mutex.c
 * @author Saad Talaat
 * @name Mutex
 * @brief Mutex synchronization primitives
 */
#include <synchronization/semaphore.h>

/**
 * @param s the base semahpore of mutex to initalize.
 * @return nothing
 * @brief initialization to mutex structs and values.
 * @details
 * A mutex is a binary semaphore thus, counting variable
 * is set to one and a list of waiting processes is initialized.
 */
void
mutex_init(semaphore_t *s){
	s->count = 1;
	wait_init(&(s->wait_list));
}

/**
 * @param s the semaphore that belongs to mutex
 * @param proc the process to block
 * @return nothing
 * @brief blocks a process over a mutex
 * @details
 * a binary clone to the semaphore down function.
 */
void
mutex_lock(semaphore_t *s, proc_t *proc){
	if(s->count == 1)
		s->count =0;
	else
		wait_sleep(&(s->wait_list), proc, 0);
}

/**
 * @param s mutex base semaphore
 * @return nothing
 * @brief sets binary resource to free
 * @details
 * a clone of semaphore up function
 */
void
mutex_unlock(semaphore_t *s){
        proc_t *waiting_proc;
        if(LIST_EMPTY(&(s->wait_list)))
                s->count ++;
        else
                wait_wakeup(&(s->wait_list));

}
/**
 @}
 */
