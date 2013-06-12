/**
 * @addtogroup Memory Management
 * @{
 * @file mm.c
 * @author Saad Talaat
 * @name Memory allocation/deallocation
 * @{
 * @brief Memory Management
 * @details
 * Memory allocation is done by iniating an early hash table
 * in which every page is stored as a bucket, within every
 * bucket the size of memory allocated on that page sequentially.
 * e.g.
 * [Bucket:1]->[heap1:4]->[heap2:32]->[heap3:1024]->...
 * in the first page 3 heaps are allocated 1st with size 4 bytes
 * 2nd with size 32 byte and third with size 1Kb.
 */

#include <mm/mm.h>

/**
 * Here's the hash entry values
 * 1- key = page number
 * 2- val = size
 */
htable_t *mm_table;
uint32_t mm_va_base;
uint32_t cur_bucket;


void kmalloc_test(void);
/**
 * @param uint32_t key is the virtual address of table entry
 * @return uint32_t the returned hashed index of given key
 * @brief hash function for allocated memory entries in heaps hashtable
 * @details
 * since heaps are ordered and traced by a hashtable a hashing function
 * to determine index of a given address into table is mandatory. such a 
 * function returns the index of page into page tables structures since
 * in a chained hashtable heaps that belong to same page exist in the
 * same linkedlist
 */


uint32_t
mm_hash(uint32_t key){
	return PGPN(key);
}

/**
 * @param uint32_t destroy is the entry or key to be destroyed
 * @return uint32_t returns 0 (indicating given entry is free)
 * @brief destroy function to free unwanted entries
 * @details
 * a free entry is identifed by the value 0, to indicate that
 * an entry is free value is simply left however a key is freed
 */
uint32_t
mm_destroy(uint32_t destroy)
{
	destroy = 0;
	return destroy;
}

/**
 * @param nothing
 * @return nothing
 * @brief memory heap manager intialization function
 * @details
 * For a heap manager to be initialized a boot time heap is
 * allocated to hold heaps entries. Early allocated heap is
 * mapped to kernel address space and virtual address base of
 * heaps is set. Tests are exectued eventually.
 */
void
mm_init(void)
{
	int size;
	mm_table = allocate( sizeof(htable_t), sizeof(htable_t));
	size = early_htable_init(mm_table, page_count,0x200, mm_hash, mm_destroy);
	printk("[*] MM: Hash Table initalized\n");
	map_segment_page(global_pgdir, (vaddr_t)mm_table, size + sizeof(htable_t), (paddr_t)KA2PA(mm_table), PAGE_WRITABLE);
	mm_va_base = cur_bucket  = 0x80000000;
	kmalloc_test();
}

/**
 * @param uint32_t va, the desired heap virtual base address
 * @param uint32_t size, the heap size to be allocated
 * @return fail -1, else return inserted base virtual address
 * @details
 * on first use of a bucket a page is allocated then iteration
 * is done until free address is reached then insertion to hash
 * table is commited. On success it returns base address.
 *
 */
static uint32_t
mm_insert(uint32_t va, uint32_t size)
{
	uint32_t flag,key,i;
	struct Page *page;
	key = 0;
	/* First time? make new page */
	if(mm_table->bucket_size[mm_hash(va)% mm_table->buckets] == 0)
	{
		flag = x86_page_alloc(&page);
		x86_page_insert(global_pgdir, page, (void *) va, PAGE_WRITABLE);
	}
	for(i = 0; i< mm_table->bucket_size[mm_hash(va) % mm_table->buckets] ; i++)
	{
		key += ((htable_node_t *)mm_table->table[mm_hash(va) % mm_table->buckets])[i].value;
	}
	if(key >= (PAGESZ-4) || (key-PAGESZ) < size)
		return -1;
	else
	{
		flag = 0;
		flag = htable_insert(mm_table, va+key, size);
		return va+key;
	}
}

/**
 * @param uint32_t size, the size of desired heap.
 * @return fails Null, success returns heap base address.
 * @details
 * Loops per each page and checks if it is already full.
 * If full use next page for next bucket. else return
 * base address.
 */
void*
kmalloc(uint32_t size)
{
	uint32_t flag, i;

	if(size < 4)
		return NULL;
	size += (size % 2);
	for(i = mm_va_base ; i<= mm_va_base+page_count*PAGESZ; i+=PAGESZ)
	{
		flag = mm_insert(i, size);
		if(flag != -1)
			return (void *)flag;
	}
	return NULL;
}

/**
 * @param void *va removes entry correspending to heap.
 * @return nothing.
 */
void
kfree(void *va)
{
	htable_remove(mm_table,(uint32_t)va);
}

/**
 * @param nothing
 * @return nothing
 * @details
 * tests abilities to remove and allocate heaps
 * in healthy manner.
 */
void
kmalloc_test(void)
{
	void *v= NULL;
	printk("[*] MM: Testing kmalloc\n");
	v = kmalloc(16);
	kfree(v);
	printk("[*] Memory allocated %p\n", v);
	asm("xchg %bx,%bx");
	return;
}
