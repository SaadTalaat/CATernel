#include <arch/x86/elf.h>
#define ELFHDR ((struct elfhdr *) 0xA0000000)
#define SECTOR 512
void Init_userspace(void);
