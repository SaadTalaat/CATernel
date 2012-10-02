#include <types.h>
#include <arch/x86/x86.h>
#include <memvals.h>
#include <string.h>
#include <arch/x86/mm/page.h>
#include <arch/x86/elf.h>
#include <init.h>
pde_t *global_pgdir;
extern struct Segdesc catgdt[];

void Init_userspace(void)
{
	map_segment_page(global_pgdir, 0xA0000000, 0x10000000 , 0x3000000 , PAGE_PRESENT | PAGE_USER);
	asm("xchg %bx,%bx");
	struct proghdr *p,*p2;	// program headers;	
	readseg((uint32_t) 0xA0000000,SECTOR*8,0/*SECTOR*126*/); // Load init elf from disk to memory
        asm("xchg %bx,%bx");
	printk("reading elf\n");

	if( ELFHDR->magic != ELF_MAGIC ) //Check if the kernel is ELF file format, if it doesn't match get the hell out
	{
		printk("[*] corrupted init");	
		return;
	}
	printk("Reading prog hdrs\n");
        asm("xchg %bx,%bx");
	p=(struct proghdr *) ( (uint8_t *) ELFHDR+ ELFHDR->phroff); // Load program segments
	p2= p + ELFHDR->phrnum;
	for (; p < p2 ; p++)
		readseg(p->vaddr,p->memsz,p->offset);
	//((void (*)(void)) (ELFHDR->entry & 0xffffff))();
	//asm("ljmp %0, %1 " ::  "i"(0x18),"a"(ELFHDR->entry));
	        asm("xchg %bx,%bx");
	printk("Jumping\n");
	asm("ljmp %0, %1 " ::  "i"(0x18),"i"(0xA0000000));


	return;	
	
}
