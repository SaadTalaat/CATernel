#ifndef _CATERNEL_INIT_H_
#define _CATERNEL_INIT_H_
#include <arch/x86/elf.h>
#include <proc/proc.h>
#define ELFHDR ((struct elfhdr *) 0xA0000000)
#define SECTOR 512
void Init_userspace(proc_t *);

#endif
