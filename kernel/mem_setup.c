/* Saad Talaat
 * CATReloaded (C) Copyrights 2011
 * http://catreloaded.net
 */

/* This code main function is to re-initalize the memory 
 * handling mechanism and setup a new GDT and new Page Dir/Table
 */
#include <types.h>
#include <memvals.h>
#include <cmos.h>
#include <x86.h>
#include <string.h>
#include <processor.h>
uint32_t max_addr,page_count;
static uint32_t mem_base,ext_base;
static char* next_free=0;
uint32_t *global_page_directory,first_page_address;
uint32_t cr3_page;
static int32_t
read_mem_size(int x){
	return cmos_get_reg(x) | (cmos_get_reg(x+1) << 8);
}
void
scan_memory(void){
	mem_base = ROUND_DOWN(read_mem_size(CMOS_SYSBASE_LSB)*KB,PAGESZ);
	ext_base = ROUND_DOWN(read_mem_size(CMOS_EXTMEM_LSB)*KB,PAGESZ);
	if(ext_base) //if there is a memory extension
		max_addr = ext_base + EXTMEM;
	else
		max_addr = mem_base;
	page_count = max_addr/PAGESZ;
	printk(" a:%d  b:%d\n",mem_base,ext_base);
	printk("Available Physicall memory start at 0x%x, extened memory =0x%x\n",mem_base,ext_base);
	printk("Available Memory size is %d Kb\n",(max_addr/KB));
	printk("Number of Pages is %d\n",page_count);
}

static void*
allocate(uint32_t n,uint32_t align){
	extern char end[];
	void *v;
	if(next_free == 0)
		next_free = end+KERNEL_ADDR;
	next_free = end;
	printk("Current pointer is at %p\n",next_free);
	next_free = ROUND_UP((next_free),align);
	v = next_free;
	next_free+=n;
	printk("next free byte is at %p\n",next_free);
	return v;
}
void
setup_mem_x86_32(void){
	uint32_t cr0;
	uint32_t * page_directory;
	page_directory = allocate(PAGESZ,PAGESZ);
	memset(page_directory,0,PAGESZ);
	global_page_directory = page_directory;
	first_page_address = PA(page_directory);
	cr0 = read_cr0();
	printk("Page Directory address is %p\n",page_directory);
	cr0 |= X86_CR0_PG |X86_CR0_PE | X86_CR0_AM | X86_CR0_WP | X86_CR0_NE | X86_CR0_MP;
	page_directory[DIROFF(VIRTPGT)] = PA(page_directory) |PAGE_PRESENT |PAGE_WRITABLE;
	page_directory[DIROFF(USERVIRTPGT)] = PA(page_directory) | PAGE_PRESENT | PAGE_USER;
	printk("VIRTPGT = %x %x %x, USERVIRTPGT = %x %x\n",page_directory[DIROFF(VIRTPGT)],DIROFF(VIRTPGT),VIRTPGT,
				page_directory[DIROFF(USERVIRTPGT)],DIROFF(USERVIRTPGT));
	page_directory[0] = page_directory[DIROFF(KERNEL_ADDR)];
	printk("DAH %x\n",page_directory[0]);
	printk("First page address is 0x%8x\n",first_page_address);
	write_cr3(first_page_address);
	write_cr0(cr0);
	asm("mov %eax,%eax");
	while(1);
//	printk("CR0 value is 0x%8x\n",cr0);

}
