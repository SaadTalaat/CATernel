#ifndef _CATERNEL_SYNCH_SPINLOCK_H_
#define _CATERNEL_SYNCH_SPINLOCK_H_
#include <arch/x86/asm.h>
typedef struct spinlock{
	uint32_t lock;
} spinlock_t;

#define spinlock_lock(lock) arch_spinlock_lock(&(lock->lock))
#define spinlock_unlock(lock) arch_spinlock_unlock(&(lock->lock))
#endif
