#include <arch/x86/bios/bios.h>
#include <types.h>
#define BIOS_EBDA_PTR  0x40e
uintptr_t  ebda = 0;
void bios_init(void)
{
	/* Copy the EBDA address out from BIOS Data Area */
	printk("Setting EBDA..\n");
	printk("EBDA_PTR -> %x \n" , *((uint16_t *)BIOS_EBDA_PTR)* 0x10);
	ebda = (uintptr_t)(*((uint16_t *) (BIOS_EBDA_PTR) ) * 0x10);
}
