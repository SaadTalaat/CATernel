#include <rdisk.h>
#include <arch/x86/x86.h>
#include <arch/x86/elf.h>

void readsect(void*,uint32_t);
void readseg(uint32_t,uint32_t,uint32_t);
void waitdisk(void);


void
readseg(uint32_t va,uint32_t count,uint32_t offset)
{
		
	uint32_t end_va;
//	__//asm __volatile("movl %%edi,%%ebx" ::"b"((uint32_t)offset));
	va &= 0xFFFFFFFF;
	printk("VA = %p\n", va);
	//asm("xchg %bx,%bx");
	end_va = va + count;
	printk("end VA = %p\n", end_va);
        //asm("xchg %bx,%bx");
	va &= ~(SECTOR -1);
        printk("VA2 = %p\n", va);
        //asm("xchg %bx,%bx");
	offset = (offset/SECTOR)+1;
	printk("Off = %p\n", offset);
        //asm("xchg %bx,%bx");
/*	if( va == 0x10000){
		offset +=1;
	}
*/	
	while(va < end_va) {
		readsect((uint8_t *)va,offset);
		va += SECTOR;
		offset++;
	}	
}

void
readsect(void *dst,uint32_t offset)
{
//	glob=offset;
        //asm("xchg %bx,%bx");

	waitdisk();
	outb(1,0x1F2);		// sector count 
	outb(offset,0x1F3); 	// sector number
	outb(offset >> 8 ,0x1F4); //Cylinder Low
	outb(offset >> 16,0x1F5); //Cylinder High
	outb( (offset >> 24) | (0xE0),0x1F6); 
	outb(0x20,0x1F7);	//Read sectors with a retry
	waitdisk();
        //asm("xchg %bx,%bx");
	
	insl(dst,SECTOR/4,0x1F0); // Load binaries from disk to dst address (ELFHDR)
        //asm("xchg %bx,%bx");

}

void
waitdisk(void){
	 while ((inb(0x1F7) & 0xC0) != 0x40);
}
