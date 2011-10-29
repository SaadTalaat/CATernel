#include <x86.h>
#include <elf.h>

#define SECTOR 512
#define ELFHDR ((struct elfhdr *) 0x10000)
#define ELF_MAGIC2 0x8ec031fc
void readsect(void*,uint32_t);
void readseg(uint32_t,uint32_t,uint32_t);
uint32_t glob;
#define ELF_PAD 0x80
void
cmain(void)
{	
	struct proghdr *p,*p2;	// program headers;
	
	readseg((uint32_t) ELFHDR,SECTOR*8,0); // Load kernel from disk to memory
	/* initialize magic to the first for characters of ELF MAGIC signature */
//	magic = ELFHDR->magic[0]+ELFHDR->magic[1]+ELFHDR->magic[2]+ELFHDR->magic[3];
	if( ELFHDR->magic != ELF_MAGIC ) //Check if the kernel is ELF file format, if it doesn't match get the hell out
		goto getout;	
	//	__asm __volatile("movl %%eax,%%esi" ::"a" (ELFHDR->magic));	
		/*__asm __volatile("movl %%eax,%%ebx" ::"a"(ELFHDR->magic ));
		__asm __volatile("movl %%eax,%%esi" ::"a"(  ));
		__asm __volatile("movl %%eax,%%ebp" ::"a"(  ));
		__asm __volatile("movl %%eax,%%ebx" ::"a"(  ));*/
		
	

	p=(struct proghdr *) ( (uint8_t *) ELFHDR+ ELFHDR->phroff); // Load program segments
	p2= p + ELFHDR->phrnum;
//	__asm __volatile("movl %%eax,%%esi" ::"a" ((p->offset)));
//	__asm __volatile("movl %%eax,%%edi" ::"a" ((p->memsz)));
//	while(1);
	for (; p < p2 ; p++)
		//LOAD THEM INTO MEMORY	
		readseg(p->vaddr,p->memsz,p->offset);
//	__asm __volatile("movl %%eax,%%esi" ::"a" ((*(uint32_t *)(0x10000c))));
	__asm __volatile("jmp	%%eax"::"a" ( (uint32_t *)((ELFHDR->entry)&0xffffff)));
	while(1);
//	((void (*)(void)) (ELFHDR->entry & 0xffffff))();
getout:
	outw(0x8A00,0x8A00);
	outw(0x8E00,0x8A00);
	while(1);	
}
void
waitdisk(void){
	 while ((inb(0x1F7) & 0xC0) != 0x40);
}
void
readseg(uint32_t va,uint32_t count,uint32_t offset)
{
		
	uint32_t end_va;
//	__asm __volatile("movl %%edi,%%ebx" ::"b"((uint32_t)offset));
	va &= 0xFFFFFF;
	end_va = va + count;	
	va &= ~(SECTOR -1);
	offset = (offset/SECTOR)+1;
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
	waitdisk();
	outb(1,0x1F2);		// sector count 
	outb(offset,0x1F3); 	// sector number
	outb(offset >> 8 ,0x1F4); //Cylinder Low
	outb(offset >> 16,0x1F5); //Cylinder High
	outb( (offset >> 24) | (0xE0),0x1F6); 
	outb(0x20,0x1F7);	//Read sectors with a retry
	waitdisk();
	
	insl(dst,SECTOR/4,0x1F0); // Load binaries from disk to dst address (ELFHDR)
}
