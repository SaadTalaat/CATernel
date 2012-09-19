/* Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */

#include <types.h>
#include <arch/x86/x86.h>
#include <memvals.h>
#include <cmos.h>
#include <string.h>
#include <arch/x86/processor.h>
#include <arch/x86/mm/page.h>
/*
 * Global variables
 */
uint32_t max_addr;
static uint32_t mem_base, ext_base;	// Base of memory, base of extended memory
char* next_free = 0;
static uint32_t alloc_lock = 0;
extern struct Page *pages;
/*
 * New global descriptor table
 * Since paging is on, 
 * Discard segment registers, and refer
 * to full memory, with different DPL
 * values.
 *
 */

struct Segdesc catgdt[] = {
	// NULL Segment
	SEG_NULL,
	// kernel code segment
	[1] = SEGMENT(0xffffffff, 0, SEGACS_RW | SEGACS_X),
	//kernel data segment
	[2] = SEGMENT(0xffffffff, 0, SEGACS_RW),
	// user code segment
	[3] = SEGMENT(0, 0xffffffff, SEGACS_RW | SEGACS_USR | SEGACS_X),
	// user data segment
	[4] = SEGMENT(0, 0xffffffff, SEGACS_RW | SEGACS_USR),
	
	//TSS Segment
	[5] = SEG_NULL


};

struct Gdtdesc gdtdesc ={
	sizeof(catgdt)-1,
	(unsigned long) catgdt

};


static uint32_t
x86_read_mem_size(int x){
	return cmos_get_reg(x) | (cmos_get_reg(x+1) << 8);
}

void
scan_memory(void){

	mem_base = ROUND_DOWN(x86_read_mem_size(CMOS_SYSBASE_LSB)*KB, PAGESZ);
	ext_base = ROUND_DOWN(x86_read_mem_size(CMOS_EXTMEM_LSB)*KB, PAGESZ);

	// is there memory extension?
	if( ext_base) // max address is the last address in extended mem
		max_addr = ext_base + EXTMEM;
	else
		max_addr = mem_base;
	// Figure out the number of pages addressable
	page_count = max_addr/PAGESZ;

	// Start informative printing

	printk("[*] Memory Base = 0x%x\n", mem_base);
	printk("[*] Extended Memory Base = 0x%x\n", ext_base);
	printk("[*] Memory size avalible = 0x%x KB\n", (max_addr - mem_base)/KB);
	printk("[*] Addressable pages = 0x%x pages\n", page_count);
	alloc_lock = 0;
}
static void*
allocate(uint32_t n,uint32_t align){
	/* since we'll start allocating pages
	 * from the end of the kernel, check
	 * linker script kerne/script.ld
	 */
	extern char end[];

	void *v;
	// is it the first time to allocate?
	if(alloc_lock == 0)
	{       next_free = end;
		alloc_lock = 1;
	}
//	next_free = end;
	// align the start address, if n is not aligned of first time
	next_free = ROUND_UP((next_free),align);
	v = next_free;
	next_free+=n;
	return v;
}

void
x86_setup_memory(void)
{
	uint32_t cr0, cr3;
	pde_t *pgdir;
	pgdir = allocate(PAGESZ, PAGESZ);
	memset( pgdir, 0 , PAGESZ);
	global_pgdir = pgdir;
	printk("[*] Page directory at %p\n",pgdir);
	cr3 = KA2PA( *((uint32_t **)&pgdir) );
	global_cr3 = cr3;
	cr0 = read_cr0();
	/*
	 * CR0 Flags to set:
	 * - Since we'll activate paging CR0_PG will do
	 * - Since we'll be operating using page tables
	 *	and in user space, refering to non-aligned
	 *	addresses is not allowed.
	 * - Remove the ability to write readonly pages
	 * - since no x87 fpu remove fpu error reporting ability
	 * - make sure we're in Protected mode
	 * - no write back caching.
	 */
	cr0 |= X86_CR0_PG | X86_CR0_AM | X86_CR0_WP | X86_CR0_NE
		 | X86_CR0_MP | X86_CR0_PE | X86_CR0_NW;

	/*
	 * refer to the page directory as a page table entry
	 * of both kernel and user pages
	 */
	pgdir[PGDIRX(VIRTPGT)] = KA2PA(pgdir) | PAGE_PRESENT |PAGE_WRITABLE;
	pgdir[PGDIRX(USERVIRTPGT)] = KA2PA(pgdir) | PAGE_PRESENT | PAGE_USER;
	
	printk("[*] VIRTPGT page directory = %x\n", pgdir[PGDIRX(VIRTPGT)]);
	printk("[*] user VIRTPGT page directory = %x\n", pgdir[PGDIRX(USERVIRTPGT)]);
	
	pages =  allocate( (sizeof(struct Page)*page_count), PAGESZ);
	printk("[*] Pages list allocated at %p\n", pages);
	x86_paging_init();
	
	// map the user space memory to pages
	map_segment_page(pgdir, (vaddr_t) USERPAGES, PAGETSZ, (paddr_t) KA2PA(pages),
				PAGE_PRESENT | PAGE_USER);
	printk("[*] Mapped segment [%p, %p] to virtual segment %p\n",
			KA2PA(pages), KA2PA(pages)+PAGETSZ, USERPAGES);


	// map the kernel stack to pages
	map_segment_page(pgdir, (vaddr_t) KERNEL_STACK_TOP - KERNEL_STACK,
			KERNEL_STACK, (paddr_t) KA2PA(kernel_stack),
			PAGE_PRESENT | PAGE_WRITABLE);
	printk("[*] Mapped segment [%p, %p] to virtual segment %p\n",
			KA2PA(kernel_stack),
			KA2PA(kernel_stack) + KERNEL_STACK,
			KERNEL_STACK_TOP - KERNEL_STACK);
	
	// to allocate the whole page table but other values panic when
	// referenced
	map_segment_page(pgdir, (vaddr_t) KERNEL_STACK_TOP - PAGETSZ,
				PAGETSZ - KERNEL_STACK, 0x0, 
				PAGE_PRESENT );
	printk("[*] Mapped segment [%p, %p] to virtual segment %p\n",
			0x0,0x0a,
			KERNEL_STACK_TOP - KERNEL_STACK);

	// map the kernel space to pages
	map_segment_page(pgdir, (vaddr_t) KERNEL_ADDR, 0x10000000, 0,
				PAGE_PRESENT | PAGE_WRITABLE);

	printk("[*] Mapped segment [%p, %p] to virtual segment %p\n",
			0x0, 0x10000000,
			KERNEL_ADDR);

	// No mapped user space so far

	x86_test_pgdir();

	/*
	 * when cr3 is set, it should read the first page table
	 * from the refered page dir, since we are still in
	 * kernel space, set first page to the mapped before
	 * kernel pages table.
	 */
	pgdir[0] = pgdir[PGDIRX(KERNEL_ADDR)];
	
	// write cr3 with the page directory address
	write_cr3(cr3);

	// enable paging
	write_cr0(cr0);
	/** At this point paging is on **/

	/* 
	 * there's no user segments set with rpl = 3
	 * and since all segment registers rpl's 0 we
	 * need to reload a new GDT table with user
	 * segments. just like we did in both
	 * boot and reattaching the kernel.
	 *
	 * However, we need to program cpu state struct,
	 * Until cpu state per kernel structure is done
	 * we'll refer directly to GDT.
	 *
	 * - DS = KERNEL SEGMENT
	 * - ES = KERNEL SEGMENT
	 * - SS = KERNEL SEGMENT
	 * - FS = USER SEGMENT
	 * - GS = USER SEGMENT
	 *
	 */
	asm volatile("lgdt gdtdesc");
	asm volatile("movw %%ax, %%ds" :: "a" (0x10));
	asm volatile("movw %%ax, %%es" :: "a" (0x10));
	asm volatile("movw %%ax, %%ss" :: "a" (0x10));
	asm volatile("movw %%ax, %%fs" :: "a" (0x20 | 3));
	asm volatile("movw %%ax, %%gs" :: "a" (0x20 | 3));
	// do the tricky tricky far jump to reload CS
	asm volatile("ljmp %0, $1f\n 1:\n" :: "i" (0x8));
	asm volatile("lldt %%ax" :: "a" (0x0));
	printk("[*] New global descriptor table set\n");
	// to avoid errornous mapping remove the 1st entry
	pgdir[0] = 0;
	write_cr3(cr3);
}
