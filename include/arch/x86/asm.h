#ifndef _CATERNEL_X86_ASM_H_
#define _CATERNEL_X86_ASM_H_
#include <arch/x86/processor.h>
static void inline arch_spinlock_lock(uint32_t *lock)
{
        register reg;
        /** TODO
         * 1- Disable scheduling
         * 2- make atomic lock as in doc
         */

        asm volatile("1:\npause\n"
                        "mov (%0), %1\n": "=r" (reg) : "m" (lock));
        asm volatile("testl %1,%1\n"
                        "jnz $1f\n"
                        "incl %1\n"
                        "xchgl %1,(%0)\n":"=m" (lock): "r" (reg));
        MFENCE;
}

static void inline arch_spinlock_unlock(uint32_t *lock)
{
        //TODO Serializing memory using memory as clobbered
        asm("movl %0,%eax\n"
                "movl $0,%ebx\n"
                "xchgl %ebx,(%eax)\n":: "m"(lock));
        MFENCE;

	//enable scheduling.
}

#endif
