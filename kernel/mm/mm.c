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
uint32_t
mm_hash(uint32_t key){
	return PGPN(key);
}

uint32_t
mm_destroy(uint32_t destroy)
{
	destroy = 0;
	return destroy;
}
void
mm_init(void)
{
	htable_t table;
	mm_table = &table;
	early_htable_init(mm_table, page_count,0x400, mm_hash, mm_destroy);
	printk("[*] MM: Hash Table initalized\n");
	mm_va_base = cur_bucket  = 0x80000000;
	kmalloc_test();
	while(1);
}

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
void
kfree(void *va)
{
	htable_remove(mm_table,(uint32_t)va);
}
void
kmalloc_test(void)
{
	void *v= NULL;
	printk("[*] MM: Testing kmalloc\n");
	printk("[*] Memory allocated %p\n", v);
	v = kmalloc(16);
	printk("[*] Memory allocated %p\n", v);
	v = kmalloc(200);
	printk("[*] Memory allocated %p\n", v);

	v = kmalloc(500);
	printk("[*] Memory allocated %p\n", v);

	v = kmalloc(300);
	printk("[*] Memory allocated %p\n", v);
	kfree(v);
	v = kmalloc(800);
	printk("[*] Memory allocated %p\n", v);
	v = kmalloc(192);
	printk("[*] Memory allocated %p\n", v);
	v = kmalloc(111);
	printk("[*] Memory allocated %p\n", v);
	v = kmalloc(50);
	printk("[*] Memory allocated %p\n", v);
	return;
}
