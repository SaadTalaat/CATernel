/* Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */

#ifndef _CATERNEL_X86_PAGE_H_
#define _CATERNEL_X86_PAGE_H_

#include <structs/linkedlist.h>
#include <memvals.h>
#include <test.h>
/*typedef struct {
	unsigned present:1;
	unsigned writable:1;
	unsigned accessible:1;
	unsigned write_through:1;
	unsigned cache_disable:1;
	unsigned accessed:1;
	unsigned ignore_this:1;
	unsigned ps:1;
	unsigned ignore_this_2:4;
	unsigned address:20;
}__attribute__((packed)) pde_t;
*/
typedef uint32_t pde_t;
typedef struct {
	unsigned present:1;
	unsigned writable:1;
	unsigned accessible:1;
	unsigned write_through:1;
	unsigned cache_disable:1;
	unsigned accessed:1;
	unsigned dirty:1;
	unsigned pat:1;
	unsigned global:1;
	unsigned ignore_this:3;
	unsigned address:20;

} __attribute__((packed)) pte_t;

// Least significant 12 bit are flags
#define PAGE_PRESENT	0x1
#define PAGE_WRITABLE   0x2
#define PAGE_USER	   0x4
#define PAGE_WTHROUGH   0x8
#define PAGE_CDISABLE   0x10
#define PAGE_ACCESSED   0x20
#define PAGE_DIRTY	  0x40

// Page Table constants
#define PAGE_DIRTY	  0x40
#define PAGE_GLOBAL	 0x100

// Page Directory constants
#define PAGE_PAGESZ	 0x80

/* Shift values */
#define PGTBL_SHIFT 12
#define PGDIR_SHIFT 22
#define PGSHIFT 12
#define PTSHIFT 22

/* Macros to parse linear address */
#define PGDIRX(lin)	(( ((uint32_t)(lin)) >> PGDIR_SHIFT) & 0x3FF)
#define VIRT_PGDIRX(lin)	PGDIRX((lin))

#define PGTBLX(lin)	(( ((uint32_t)(lin)) >> PGTBL_SHIFT) & 0x3FF)
#define VIRT_PGTBLX(lin)	PGTBLX((lin))

#define PGPN(lin)	(((uint32_t)(lin)) >> PGTBL_SHIFT)
#define VIRT_PGPN(lin)	PGPN((lin))

#define PGOFF(lin)	(( (lin) & 0xFFF))
#define PTD_ADDR(lin)	(( (uint32_t) (lin)) & ~0xFFF)
/* Macros for converting addresses */

/* It takes physical address and return kernel 
 * virtual address
 */
#define PA2KA(pa)\
({					\
	paddr_t m_pa = ((paddr_t) pa);	\
	paddr_t m_ppn = PGPN(m_pa);	\
	if( m_ppn >= page_count){	\
		panic("PA2KA called with invalid paddr\n");\
		}			\
	(void *) (m_pa + KERNEL_ADDR);	\
})

#define KA2PA(va)\
({					\
	vaddr_t m_va = (vaddr_t) (va);	\
	if( m_va < KERNEL_ADDR){\
		panic("KA2PA called with bad address \n");\
	}\
	m_va - KERNEL_ADDR;\
})

/* 
 * This Macro returns paddr of page containing
 * the given virtual address
 *
#define VA2PA(pgdir, va)\
({					\
	pte_t *pte;			\
	pgdir = &pgdir[PGDIRX(va)];	\
	if( !(*pgdir & PAGE_PRESENT))	\
		( (paddr_t) ~0x0);	\
	pte = ( (pte_t *) PA2KA(pgdir->address))	\
	if(!(pte[PGTBLX(va)] & PAGE_PRESENT))		\
		( (paddr_t) ~0x0);	\
	else				\
		pte[PGTBLX(va)]->address;	\
})
*/
extern size_t page_count;
static inline paddr_t
va2pa(pde_t *pgdir, void *va){
	uint32_t *pte;
	
	pgdir = &pgdir[PGDIRX(va)];
	if( !(*pgdir & PAGE_PRESENT))
	{
		printk("NO PAGE DIR\n");
		return ~0;

	}
	pte = (uint32_t *) PA2KA( (*pgdir & ~0xFFF));
	if( !(pte[PGTBLX(va)] & PAGE_PRESENT)){
		printk("NO PAGE TABLE\n");
		return ~0;
	}
	return (pte[PGTBLX(va)] & ~0xFFF);
/*					 
	pte_t *pte;		     
	pgdir = &pgdir[PGDIRX(va)];     
	if( !( (*(pgdir)) & PAGE_PRESENT))   
		return ( (paddr_t) ~0x0);	  

	pte = ( (pte_t *) PA2KA( (*pgdir & ~0xFFF)));	
	if(!( (*((uint32_t *) pte+PGTBLX(va))) & PAGE_PRESENT))	 
		return ( (paddr_t) ~0x0);      			    
	return *((paddr_t *) &pte[PGTBLX(va)]);       
*/
}

/*
 * Page table structure
 *
typedef struct {
	unsigned present:1;
	unsigned writable:1;
	unsigned accessible:1;
	unsigned write_through:1;
	unsigned cache_disable:1;
	unsigned accessed:1;
	unsigned ignore_this:1;
	unsigned ps:1;
	unsigned ignore_this_2:4;
	unsigned address:20;
}__attribute__((packed)) pde_t;

typedef struct {
	unsigned present:1;
	unsigned writable:1;
	unsigned accessible:1;
	unsigned write_through:1;
	unsigned cache_disable:1;
	unsigned accessed:1;
	unsigned dirty:1;
	unsigned pat:1;
	unsigned global:1;
	unsigned ignore_this:3;
	unsigned address:20;

}__attribute__((packed)) pte_t;
*/
extern volatile pte_t	virtpgt[];
extern volatile pde_t	virtpgd[];
extern char kernel_stack[], kernel_stack_end[];
extern struct Page *pages;
extern uint32_t global_cr3;
extern pde_t *global_pgdir;
extern struct Segdesc gdt[];
char *next_free;
/* since we need a way to loop through the page directories
 * and tables ..etc we'll use a linked list to link pages 
 * togeather and audit them
 */
LIST_HEAD(PageList, Page);
typedef LIST_ENTRY(Page) page_entry_t;

struct Page{

	page_entry_t link;	// the link structure
	uint16_t ref;		// number of pointers using the page
};

/* define inline functions for page to address
 * and index conversions
 * I hope you know what is inline functions.
 */

/*
 * Get Page number
 */
static inline uint32_t
pagetoppn(struct Page *p)
{
	return p-pages;
}

/*
 * get a page refering to a paddr
 */
static inline void*
patopage(paddr_t p)
{
	
	if(PGPN(p) >= page_count){
	    panic("patopage called with invalid physaddr\n");
		return;
	}
	return &pages[PGPN(p)];
}
/*
 * get a paddr a page refering to
 */
static inline paddr_t
pagetopa(struct Page *p)
{
	return pagetoppn(p) << PGSHIFT;
}
static inline void*
pagetova(struct Page *p)
{
	return  (void *)PA2KA(pagetopa(p));

}

/* Main operational functions */
void x86_paging_init(void);
void x86_page_init(struct Page *);
int x86_page_alloc(struct Page **);
void x86_page_free(struct Page *);
void x86_page_detach(struct Page *);
pte_t* x86_pgdir_find(pde_t*, const void *, int);
struct Page* x86_page_lookup(pde_t*, void *, pte_t **);
void x86_page_remove(pde_t*, void*);
int x86_page_insert(pde_t*, struct Page*, void *, uint32_t);
void map_segment_page(pde_t*, vaddr_t, size_t, paddr_t, int);

/* Testing purpose functions */
void x86_test_pgdir(void);
void x86_test_paging(void);
static void x86_test_page_alloc(void);

#endif	/* _CATERNEL_X86_PAGE_H_ */
