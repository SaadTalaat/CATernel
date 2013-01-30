#include <arch/x86/x86.h>
#include <arch/x86/elf.h>
/**
 * @addtogroup x86-boot
 * @{
 */
/**
 * @file main.c
 * @author Saad Talaat 
 * @date 26/10/2011
 * @brief Elf binary structures and constants, for parsing binaries.
 *
 * Loads the kernel from image.
 * includes an elf parsing and loading for loading the kernel from disk, which
 * is done by reading the disk at offset 512 = 1 sector, and loading it into 
 * memory using readsect and readseg respectively.
 * after loading the kernel image into memory, the LOAD program header is loaded
 * into cs and jumps to kernel entry point.
 * Here, we use the <b>Fixed Disk Controller</b> device which's ports are
 * from 0x1F0 ~ 0x1F7
 *
 */

/**
 * @name Bootloader-KernelLoading
 * @{
 */
/**
 * @def SECTOR
 * @brief equals the sector size in bytes
 */
#define SECTOR 512
/**
 * @def ELFHDR
 * @brief The kernel physical address to load image into
 */
#define ELFHDR ((struct elfhdr *) 0x10000)

/**
 * @fn void readsect(void *, uint32_t);
 * @brief reads disk at a given offset
 * @param void* va, is the address to load read data into
 * @param uint32_t offset, offset in image
 * @return nothing.
 *
 */
void readsect(void*,uint32_t);
/**
 * @fn void readseg(uint32_t, uint32_t, uint32_t);
 * @brief loads data from disk into memory with data size to read.
 * @param uint32_t va, virtual address to load data into
 * @param uint32_t count, the number of sectors to load into memory.
 * @param uint32_t offset, the offset of data into disk.
 * @return nothing.
 *
 * It could be called as a wrapper function to readsect, since readseg 
 * uses readsect int its operations.
 */
void readseg(uint32_t,uint32_t,uint32_t);


/**
 * @fn void cmain(void);
 * @brief reads and validates kernel image and loads it and fetch it into IP.
 * @return nothing.
 *
 * this function is called by the boot.S after setting up segmentation and
 * operating in protected mode. it loads the kernel image and validate that
 * it's an ELF image, then loads the program headers into memory. and jump
 * to kernel image, if ever a kernel return it will spin or hang. and such
 * thing is merely impossible.
 */
void
cmain(void)
{	
	struct proghdr *p,*p2;	// program headers;
	
//	readseg((uint32_t) ELFHDR,SECTOR*8,0); // Load kernel from disk to memory
	/**
	 * initialize magic to the first for characters of ELF MAGIC signature 
	 */
	if( ELFHDR->magic != ELF_MAGIC ) //Check if the kernel is ELF file format, if it doesn't match get the hell out
		goto getout;	
		
	

	p=(struct proghdr *) ( (uint8_t *) ELFHDR+ ELFHDR->phroff); // Load program segments
	p2= p + ELFHDR->phrnum;
	for (; p < p2 ; p++)
		//LOAD THEM INTO MEMORY	
		readseg(p->vaddr,p->memsz,p->offset);
	((void (*)(void)) (ELFHDR->entry & 0xffffff))();
getout:
	outw(0x8A00,0x8A00);
	outw(0x8E00,0x8A00);
	while(1);	
}

/**
 * @fn void waitdisk(void);
 * @brief hangs till disk operation is done.
 * @return nothing.
 * 
 * this function just reads the disk port status register
 * holds the value of 0x40 which means disk is not executing
 * a command and ready.
 */

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

/** @}
 */ 
