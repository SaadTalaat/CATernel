/* Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */
/**
 * @addtogroup Memory-Management
 * @{
 * @file page.c
 * @date Wednesday 19/09/2012
 * @brief Intel Paging.
 * @name Paging.
 * @{
 * @details Programming the hardware support, Initially 32-bit paging is used.
 * and Segment register refer to the whole memory since we'll be operating on
 * protected paging memory mode. and Kernel is mapped to Virtual address 0xF0000000.
 * Paging is set recursively by making cr3 refer to the paging directory.
 */
#include <types.h>
#include <arch/x86/x86.h>
#include <memvals.h>
#include <string.h>
#include <arch/x86/mm/page.h>

uint32_t page_count;
pde_t *global_pgdir;
uint32_t global_cr3;
extern char* next_free;
// Physical pages
struct Page *pages;
// free physical pages
static struct PageList free_pages;


/**
 * 
 * @brief Initiate global pages
 * @details 1-initalize the global free pages, since they
 * all free at the start. whatever that means.\n
 * 2- Loop around the page list and map it to the free
 * pages.\n
 * 3- Remove I/O Hub Mapping.
 */
void
x86_paging_init(void){
	int count;
	extern char end[];
	struct Page *io_start, *io_end;

	LIST_INIT(&free_pages);


	for(count =0; count< page_count; count++)
	{
		// since they're newly paged, they refer to nothing
		pages[count].ref = 0;
		// put that page into the pages list start
		LIST_INSERT_HEAD(&free_pages, &pages[count], link);
	}

	/**
	 * we need to remove the io hole from pages
	 */
	LIST_REMOVE(&pages[0], link);
	pages[0].ref = 1;
	// get the start page of io gap
	io_start = patopage(IOPHYMEM);
	// until we reach the kernel start
	io_end = patopage(KA2PA(next_free));
	
	// Loop on these pages and set them reserved
	while( io_start <= io_end)
	{
		LIST_REMOVE(io_start, link);
		io_start->ref = 1;
		io_start++;
	}

}

/**
 * @param struct Page*, reference to page that to be initialized.
 * @brief Initiate a page entry.
 * @details Page memory is set to Zeros.
 */
void
x86_page_init(struct Page *page)
{
	memset(page,0, sizeof(page));
}

/**
 * @param struct Page** reference to page pointer.
 * @return 0 if success, -1 if fails 
 * @brief Allocate a page
 * @detials page is removed from the free pages list
 * and initiated.
 */
int
x86_page_alloc(struct Page **page_byref){
	
	// Get a free page
	struct Page *p = LIST_FIRST(&free_pages);

	// isn't there any free pages left?
	if(p == NULL)
		return -1;
	// remove a free page, since we'll allocate it
	LIST_REMOVE(p,link);
	x86_page_init(p);
	// set the given page to the allocate page
	*page_byref = p;
	return 0;
}

/**
 * @param struct Page*, the page to free
 * @brief given page is freed.
 * @details A page is freed by inserting it into the
 * free pages list.
 */
void
x86_page_free(struct Page *page)
{
	if(page == NULL)
		return;
	// just slap it in the free pages list
	LIST_INSERT_HEAD(&free_pages, page, link);
}

/**
 * @param struct Page* page to detach from calling execusion stream.
 * @brief when [un]refering a page, ref member is decremented.
 */
void
x86_page_detach(struct Page *page)
{
	if(page == NULL)
		return;
	page->ref--;
	// if it has no other pointer attached, free it
	if(page->ref == 0)
		x86_page_free(page);
	
	return;
}

/**
 * @param pde_t Page directory to search into.
 * @param const void* virtual address to allocate pages for.
 * @param int allocation flag, if set allocate a page.
 * @return pte_t* page table containing the created or existing page.
 * @brief find a page, if doesn't exist allocate it
 * @details
 * the function searches for the page table containing the page that
 * refers to a given virtual address, if it doesn't exist and allocate
 * flag is unset, NULL is returned. If it doesn't exist and allocate
 * flag is set, page is created and it's parent page table is returned.
 */
pte_t*
x86_pgdir_find(pde_t *pgdir, const void *va, int allocate)
{
	// get the directory entry refering to that vaddr
	struct Page *p;

	if( (pgdir[PGDIRX(va)] & PAGE_PRESENT) != 0)
		return &( (pte_t *) (PA2KA(PTD_ADDR(pgdir[PGDIRX(va)]))))[PGTBLX(va)];
	else if( allocate == 0)
		return NULL;
	else
	{
		if(x86_page_alloc(&p) < 0)
			return NULL;
		p->ref = 1;
		memset(pagetova(p), 0, PAGESZ);
		pgdir[PGDIRX(va)] = pagetopa(p) | PAGE_PRESENT | PAGE_USER | PAGE_WRITABLE;
		return &((pte_t *) (PA2KA(PTD_ADDR(pgdir[PGDIRX(va)]))))[PGTBLX(va)];
	}
	panic("Page find couldn't page, or allocate a page\n");
	return NULL;

	
}

/**
 * @param pde_t page directory to search.
 * @param void* virtual address of desired page.
 * @param pte_t** page table reference to set.
 * @return the found page.
 * @brief looks up a page refers to vaddr.
 * @details
 * the functions looks up for a page into a page directory
 * and return back to values in two different manner.\n
 * it returns the desired page address if found. and the page
 * table address is set to a given paramter.
 */
struct Page*
x86_page_lookup(pde_t *pgdir, void *va, pte_t **pte)
{
	pte_t *pt;
	// find that table in pgdir
	pt = x86_pgdir_find(pgdir, va, 0);
	// does it exist?
	if(pt == NULL)
		return NULL;
	if(pte != NULL)
		*pte = pt;
	if( (*((uint32_t *)pt)) & PAGE_PRESENT)
		return patopage(pt->address);
	return NULL;
}

/**
 * @param pde_t* page directory to remove page from.
 * @param void* virtual address the page refers to.
 * @brief removes a page that refers to a given Virtual address from a page directory.
 * @details
 * a virtual address is looked up in a page directory and detached. then the
 * TLB (Translate Lookaside Buffer) is updated to avoid misbehaviour or page faults.
 */
void
x86_page_remove(pde_t* pgdir, void* va)
{
	pte_t *pte;
	uint32_t zero;
	struct Page *page = x86_page_lookup(pgdir, va, &pte);
	if (page == NULL)
		return;
	x86_page_detach(page);
	zero = 0;
	*pte = *((pte_t *) &zero);
	invlpg(va);
	return;
}

/**
 * @param pde_t* page directory to insert page into.
 * @param struct Page*, the page to insert.
 * @param void* Virtual address to use.
 * @param uint32_t permissions on a page.
 * @return 0 if success, -1 if fail.
 * @brief inserts a page that refers to a given va, into a given page directory.
 * @details
 * a previously set page is inserted into a page directory and refer to
 * a given virtual address no matter what physical address is, and permission
 * flags are set on a page.
 */
int
x86_page_insert(pde_t *pgdir, struct Page *page, void* va, uint32_t perm)
{

	pte_t *pte = x86_pgdir_find(pgdir, va, 1);
	if(pte == NULL)
		return -1;

	if( *((uint32_t *) pte) & PAGE_PRESENT)
	{
		if(PTD_ADDR(*((uint32_t *)pte)) != pagetopa(page))
			x86_page_remove(pgdir, va);
		else
			page->ref--;

	}

	*((uint32_t *) pte) = pagetopa(page) | perm | PAGE_PRESENT;
	page->ref++;
	invlpg(page);
	return 0;

/*
	pte_t *pte = x86_pgdir_find(pgdir, va, 1);
	
	if( pte == NULL)
		return -1;
	page->ref ++;
	if ( *((uint32_t *) pte) & PAGE_PRESENT)
		x86_page_remove(pgdir, va);
	
	pte = x86_pgdir_find(pgdir, va, 1);
	
	if(pte == NULL)
	{
		page->ref--;
		return -1;
	}
	paddr_t pa = pagetopa(page);
	pte->address = pa;
	pte->present = 1;
	*((uint32_t *) pte) |= perm;
	*((uint32_t *) &pgdir[PGDIRX(va)]) |= perm;
	return 0;
*/
}

/**
 * @param pde_t* page directory to use.
 * @param vaddr_t virtual address to use.
 * @param size_t segment size.
 * @param paddr_t physical address of segment start.
 * @param int page permissions.
 * @brief maps a segment to a virtual address on a specific page directory.
 */
void
map_segment_page(pde_t *pgdir, vaddr_t linear, size_t size, 
			paddr_t physical, int perm)
{
	uint32_t count;
	uint32_t *pte;
	for(count = 0; count < size; count+=PAGESZ)
	{
		//printk("Calling pgdir_find with va %x\n", count+linear);
		pte =(uint32_t *) x86_pgdir_find(pgdir, (void *) (linear+count), 1);
		*pte = (physical+count) | PAGE_PRESENT | perm;
	}



}
/**
 * test the global page directory
 */
void
x86_test_pgdir(void){

	uint32_t count, size;
	pde_t *pgdir;
	// store the global pgdir
	pgdir = global_pgdir;

	// get the size of physical pages array
	size = ROUND_UP(page_count*sizeof(struct Page), PAGESZ);

	/* 
	 * test that the physical address refered 
	 * in pgdir is the same as the pages array
	 */
	for(count = 0; count < size; count += PAGESZ){
		assert( (va2pa(pgdir, (void *)USERPAGES+count)) == (KA2PA(pages)+count));
	}
	printk("[*] TEST: pages mapped correctly\n");
	
	/*
	 * test if refering directly to the va will match
	 */

	for(count = 0; count < 0x10000000; count+=PAGESZ)
		assert( (va2pa(pgdir, (void *) KERNEL_ADDR+count)) == count);
	printk("[*] TEST: Kernel space is mapped correctly\n");

	/*
	 * Check kernel stack
	 */
	for(count = 0; count < KERNEL_STACK; count +=PAGESZ)
		assert( (va2pa(pgdir, (void *) KERNEL_STACK_TOP - KERNEL_STACK + count))
				== (KA2PA(kernel_stack) + count));
	printk("[*] TEST: Kernel stack is mapped correctly\n");

	
	/*
	 * check for mal-linked pages
	 */
	for(count = 0; count <PAGECNT; count++)
	{
		switch(count)
		{
		/*
		 * in case we're indexing either:
		 * - Kernel level pages table
		 * - User level pages table
		 * - Kernel stack page table
		 * - User pages
		 * if there's no page directory refering
		 * to them it's a fucking disaster!
		 * 
		 * if the page director is refering
		 * to a kernel level address and
		 * it's 0 panic
		 * else it is free, it should be
		 */
		case PGDIRX(VIRTPGT):
		case PGDIRX(USERVIRTPGT):
		case PGDIRX(KERNEL_STACK_TOP -1):
		case PGDIRX(USERPAGES):
		case PGDIRX(PROC_LIST):
//		case PGDIRX(USERSTART):
			assert( *((uint32_t *) &pgdir[count]) );
			break;
		default:
			if( count >= PGDIRX(KERNEL_ADDR)){
				assert( *((uint32_t *) &pgdir[count]) );
			}
			else{
				assert((*((uint32_t *) &pgdir[count])) == 0);
			}
			break;
		}
	}
	printk("[*] TEST: Page directory test passed!\n");
}

/**
 * @}
 * @}
 */
