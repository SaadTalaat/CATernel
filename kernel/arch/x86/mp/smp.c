#include <types.h>
#include <arch/x86/mp/smp.h>
#include <arch/x86/bios/bios.h>
#include <arch/x86/mm/page.h>

fpstruct_t * fs;

uint8_t fps_check(uint8_t *base)
{
	uint32_t i;
	uint8_t sum;
	
	for (i = 0, sum = 0; i < 16; i++)
		sum = (uint8_t) (sum + base[i]);
	if (sum == 0 ) return 0;
	else
	return 1;
}

void fsp_print(fpstruct_t * fs)
{
	printk("Signature %s \n " , fs->Signature);
	printk("Configuration Table paddr %x \n", fs->config_addr);
	printk("Table Lenght %x \n" , fs->len);
	printk("Version %x \n" , fs->version);
	printk("Feature1 %x \n" , fs->feature1);
	printk("Feature2 %x \n" , fs->feature2);
}

/**
 * @fn void find_set_fps(void);
 * @brief Searches and Sets the global Floating pointer structure.
 *  1. search first 1K of EBDA
 *  2. if EBDA is undefined, search last 1K of base memory
 *  3. search 64K starting at 0xf0000
 */
void find_set_fps(void)
{
	printk("[*] Searching for FPS...\n");
	asm("xchg %bx,%bx");
	uint8_t * addr[2] = { NULL, (uint8_t *) PA2KA(0xf0000) };
	uint32_t i;
	uint32_t j;
	uint32_t length[2] = { 1024, 64 * 1024 };	
	addr[0] = (uint8_t *) PA2KA(ebda ? (uint32_t)ebda : 639 * 1024);
	for (i = 0; i < 2; i++) {
		for (j = 0; j < length[i]; j += 16) {
			if ((*((uint32_t *) &addr[i][j]) ==
			    FS_SIGNATURE) && (fps_check(&addr[i][j])==0)) {
				fs = (fpstruct_t *) &addr[i][j];
				goto fs_found;
			}
		}
	}
	printk("[*] Failed to locate Floating point structure. \n");
	return;	
	fs_found:
	printk("[*] Located Floating point structure at %x \n" , fs);
	fsp_print(fs);
	asm("xchg %bx,%bx");
}




