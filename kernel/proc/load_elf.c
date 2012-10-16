#include <types.h>
#include <arch/x86/elf.h>
#include <memvals.h>
#include <proc/proc.h>
#include <rdisk.h>

uint32_t
elf_load_to_proc(proc_t *proc, uint32_t offset)
{

	struct elfhdr *elf_hdr = (struct elfhdr *)0xA0000000;
	struct proghdr	*p1,*p2;
	int count;
	struct Page *page;
	x86_page_alloc(&page);

	printk("PGIDR %x\n", proc->cr3);
//	x86_pgdir_find(proc->page_directory, (void *)(0xA0000000), 1);
	x86_page_insert(proc->page_directory, page, (void *) 0xA0000000, PAGE_USER | PAGE_WRITABLE); 
	write_cr3(proc->cr3);
	readseg( (uint32_t) elf_hdr, SECTOR*8, offset);
	if( elf_hdr->magic != ELF_MAGIC )
		return -1;
	p1 = (struct proghdr *) ( (uint8_t *) elf_hdr + elf_hdr->phroff);
	p2 = p1 + elf_hdr->phrnum;
	/* Change address space */
	for(; p1 < p2; p1++)
	{
		if( p1->type == PRG_LOAD)
		{
				printk("P! VA %p\n", p1->vaddr);
				proc_alloc_mem(proc, p1->vaddr, p1->memsz);
				readseg(p1->vaddr,p1->memsz, p1->offset + offset);
		}
	}
	proc->eip = elf_hdr->entry;
	map_segment_page(proc->page_directory, USERSTACK_TOP - PAGESZ, PAGESZ, 0x22000000, PAGE_USER | PAGE_PRESENT | PAGE_WRITABLE);
	write_cr3(global_cr3);
}
