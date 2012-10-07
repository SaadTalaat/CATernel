#include <types.h>
#include <arch/x86/x86.h>
#include <memvals.h>
#include <string.h>
#include <arch/x86/mm/page.h>
#include <arch/x86/elf.h>
#include <proc/proc.h>
#include <init.h>


pde_t *global_pgdir;
extern struct Segdesc catgdt[];
extern proc_t processes[];


void Init_userspace(proc_t *proc)
{
	
	map_segment_page(global_pgdir, 0xA0000000, 0x1000000 ,
				 0x20000000 , PAGE_PRESENT | PAGE_USER | PAGE_WRITABLE);
	

	struct proghdr *p,*p2;	// program headers;	
	readseg((uint32_t) 0xA0000000,SECTOR*8,SECTOR*127); // Load init elf from disk to memor
	printk("reading elf\n");

	if( ELFHDR->magic != ELF_MAGIC ) 
	//Check if the kernel is ELF file format, if it doesn't match get the hell out
	{
		printk("[*] corrupted init %8x\n", ELFHDR->magic);	
		while(1);
	}
	printk("Reading prog hdrs\n");
	p=(struct proghdr *) ( (uint8_t *) ELFHDR+ ELFHDR->phroff); // Load program segments
	p2= p + ELFHDR->phrnum;
	for (; p < p2 ; p++)
		readseg(p->vaddr,p->memsz,(SECTOR*127) + p->offset);
	printk("Jumping to %p\n", *(uint32_t *)ELFHDR->entry);
	proc->eip = ELFHDR->entry;
	proc->ss = 0x20 | 3;
	switch_address_space(proc);
	return;	
	
}
