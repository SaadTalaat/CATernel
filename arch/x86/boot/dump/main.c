#include <x86.h>
#include <elf.h>

#define SECTOR 512
#define ELFHDR ((elfhdr *) 0x10000)
void readsect(void*,uint32_t);
void readseg(uint32_t,uint32_t,uint32_t);

void
main0(void)
{	
	uint32_t magic; //place to store magic sign of ELF to check it
	struct proghdr *p,*p2;	// program headers;
	
	readseg((uint32_t) ELFHDR,SECTOR*8,0); // Load kernel from disk to memory
	
	magic = ELFHDR->magic[0]+ELFHDR->magic[1]+ELFHDR->magic[2]+ELFHDR->magic[3];

	if(magic != ELF_MAGIC) //Check if the kernel is ELF file format
		goto getout;

	p=(struct proghdr *) ( (uint8_t *) ELFHDR+ ELFHDR->phroff); // Load program segments
	p2= p + ELFHDR->phrnum;

	for (; p < p2 ; p++)
	{
		readseg(p->vaddr,p->memsz,p->offset);
	}

	//call the kernel
	((void (*)(void)) (ELFHDR->entry & 0xFFFFFF))();

getout:
	outw(0x8A00,0x8A00);
	outw(0x8E00,0x8A00);
	while(1);
	
}

void
readseg(uint32_t va,uint32_t count,uint32_t offset)
{
	uint32_t end_va;
	
	va &= 0xFFFFFF;
	end_va = va + count;
	
	va &=~ (SECTOR -1);
	offset = (offset/SECTOR)+1;

	while(va < end_va) {
		readsect((uint8_t *)va,offset);
		va+=SECTOR;
		offset++;
	}	
}

void
readsect(void *dst,uint32_t offset)
{
	waitdisk();
	outb(1,0x1F2);		// sector count 
	outb(offset,0x1F3); 	// sector number
	outb(offset >> 8 ,0x1F4); //Cylinder Low
	outb(offset >> 16,0x1F5); //Cylinder High
	outb(offset >> 24,0x1F6); 
	outb(0x20,0x1F7);	//Read sectors with a retry
	waitdisk();

	insl(dst,SECTOR/4,0x1F0); // Load binaries from disk to dst address (ELFHDR)
}
