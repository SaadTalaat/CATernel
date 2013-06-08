/**
 * @addtogroup Process-Management
 * @{
 * @file load_elf.c
 * @author Saad Talaat
 * @date Tuesday 16/10/2012
 * @brief Elf binary loader
 * @name Binary Loader.
 * @{
 */
#include <types.h>
#include <arch/x86/elf.h>
#include <memvals.h>
#include <proc/proc.h>
#include <rdisk.h>

/**
 * @brief loads a binary into a proc from an offset into image.
 * @param proc_t* proc to load binary into
 * @param uint32_t offset of the binary into image
 * @return 0 if success
 * @details
 * The binary loader loads a binary image into the 0xA000000 VA
 * where all procs elf are loaded.\n
 * Program headers are loaded into proc page directory and eip is set.
 */
uint32_t
elf_load_to_proc(proc_t *proc, uint32_t offset)
{

	struct elfhdr *elf_hdr = (struct elfhdr *)0xA0000000;
	struct proghdr	*p1,*p2;
	int count;
	struct Page *page;


	x86_page_alloc(&page);
	asm("xchg %bx,%bx");
//	x86_pgdir_find(proc->page_directory, (void *)(0xA0000000), 1);
	x86_page_insert(proc->page_directory, page, (void *) 0xA0000000, PAGE_USER | PAGE_WRITABLE); 
	write_cr3(proc->cr3);
	readseg( (uint32_t) elf_hdr, SECTOR*8, offset);

	if( elf_hdr->magic != ELF_MAGIC )
	{
		write_cr3(global_cr3);
		return -1;
	}
	p1 = (struct proghdr *) ( (uint8_t *) elf_hdr + elf_hdr->phroff);
	p2 = p1 + elf_hdr->phrnum;
	/* Change address space */
	for(; p1 < p2; p1++)
	{
		if( p1->type == PRG_LOAD)
		{
				proc_alloc_mem(proc, p1->vaddr, p1->memsz);
				readseg(p1->vaddr,p1->memsz, p1->offset + offset);
		}
	}
	proc->eip = elf_hdr->entry;
	printk("entry[%p]\n",proc->eip);
	map_segment_page(proc->page_directory, USERSTACK_TOP - PAGESZ, PAGESZ, 0x2200000, PAGE_USER | PAGE_PRESENT | PAGE_WRITABLE);
	printk("Allocating Stack: Start->[%x] End->[%x]\n",USERSTACK_TOP - PAGESZ, USERSTACK_TOP );
	printk("size %x\n", sizeof(proc_t));
	map_segment_page(proc->page_directory, (vaddr_t) proc, sizeof(proc_t), (paddr_t) KA2PA(proc), PAGE_USER | PAGE_PRESENT);
	asm("xchg %bx,%bx");
	write_cr3(global_cr3);
	printk("mapped PEB[%x] to proc\n",proc);
	return 0;
}

/**
 * @} @}
 */
