#include <arch/x86/x86.h>
#include <arch/x86/elf.h>

#define SECTOR 512
#define ELFHDR ((struct elfhdr *) 0x10000)
#define ELF_MAGIC2 0x8ec031fc

void readsect(void*,uint32_t);
void readseg(uint32_t,uint32_t,uint32_t);
void waitdisk(void);
