/* Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
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


/*
 * Initiate global pages
 */
void
x86_paging_init(void){
	int count;
	extern char end[];
	struct Page *io_start, *io_end;

	/*
	 * newly paged nervousness, initalize 
	 * the global free pages, since they
	 * all free at the start. whatever that
	 * means.
	 */
	LIST_INIT(&free_pages);

	/*
	 * Loop around the free memory
	 * and map it to the free pages
	 * Just set the whole fucking memory paged!
	 */
	for(count =0; count< page_count; count++)
	{
		// since they're newly paged, they refer to nothing
		pages[count].ref = 0;
		// put that page into the pages list start
		LIST_INSERT_HEAD(&free_pages, &pages[count], link);
	}

	/*
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

/* Initiate a page entry */
void
x86_page_init(struct Page *page)
{
	memset(page,0, sizeof(page));
}

/* 
 * Allocate a page
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

void
x86_page_free(struct Page *page)
{
	if(page == NULL)
		return;
	// just slap it in the free pages list
	LIST_INSERT_HEAD(&free_pages, page, link);
}

/*
 * when [un]refering a page, ref member is decremented
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

/* find a page, if doesn't exist allocate it */
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
	panic("Page find couldn't nor find, or allocate a page\n");
	return NULL;

/*	struct Page *p;
	uint32_t pde_i;
	pde_t pde = pgdir[PGDIRX(va)];
	pte_t *pgtbl;
	
	if( pde & PAGE_PRESENT)
	{
		pgtbl = (pte_t *) PA2KA(pde & ~0xFFF);
		return &pgtbl[ PGTBLX(va)];
	}
	
	if( allocate == 0)
		return NULL;

	if(x86_page_alloc(&p) < 0)
		return NULL;
	p->ref = 1;
	pgtbl = (pte_t *) pagetopa(p);
	paddr_t tbladdr = (paddr_t) pgtbl;

	pde_i = (tbladdr | PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER);
	pde = *((pde_t *) &pde_i);
	pgdir[PGDIRX(va)] = pde;
	printk("TABLE %p\n", tbladdr);
	memset(PA2KA(tbladdr), 0, sizeof( PAGESZ));
	
	return PA2KA((paddr_t) (pgtbl+ PGTBLX(va)));
*/
}

/*
 * looks up a page refers to vaddr
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
int
x86_page_insert(pde_t *pgdir, struct Page *page, void* va, uint32_t perm)
{
	pte_t *pte = x86_pgdir_find(pgdir, va, 0);
	
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
}

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
		//printk("%p %x\n", pte, *pte);
	}



}
/*
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

