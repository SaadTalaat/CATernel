/** @addtogroup Main
 * @{
 */

/**
 * @file kernel/play.c
 * @author Saad Talaat
 * @date 12/9/2011
 * @brief kernel main file
 *
 */

#include <types.h>
#include <cli.h>
#include <stdio.h>
#include <video.h>
#include <cmos.h>
#include <arch/x86/x86.h>
#include <cpuid.h>
#include <proc/proc.h>
#include <test.h>
#include <multiboot.h>
#include "../vfs/vfs.h"
#include "../vfs/initrd.h"
#include <drivers/i8259.h>
#include <kconsole.h>
#include <mm/mm.h>
/**
 * @fn void play(void);
 * @brief this function holds initializations and setting up facilities
 *
 * @fn void time_print(void);
 * @brief a completely useless function that prints current time.
 *
 * @fn void bootup(void);
 * @brief booting up operations
 */
void play(void);
static const char* error_panic = NULL;

void
_panic_(const char *file, int nline, const char *fmt, ...){
        va_list ap;
        if(error_panic == NULL)
                goto panic_end;
        error_panic = fmt;

        va_start(ap, fmt);
        printk("kernel panic at %s:%d:",file,nline);
        vprintk(fmt, ap);
        printk("\n");
        va_end(ap);

        panic_end:
                while(1);


}
void
time_print(void){
	char *Month[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char *Day[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	char* month_result;
	uint8_t month= 0x0;
	month = cmos_get_reg(RTC_MONTH);
	if((month & ~0xf)){
		month = (month & 0xf)+10;
	}
        printk("Date: ");
        printk("20%x",(uint32_t) cmos_get_reg(RTC_YEAR));
        printk(" %d",month);
        printk(", %x",(uint32_t) cmos_get_reg(RTC_DAY_MONTH));
	printk(" %s\n",Day[cmos_get_reg(RTC_DAY_WEEK)-1]);

}

uint8_t initrd(multiboot_info_t * mboot_ptr , uint32_t *  initrd_location_ptr , uint32_t * initrd_end_ptr)
{	

	multiboot_mod_t * mod_ptr;
	printk("[*] Modules loaded-> %d \n" , mboot_ptr->mods_count);
	asm("xchg %bx,%bx");
 	if(mboot_ptr->mods_count > 0){
	mod_ptr= PA2KA(mboot_ptr->mods_addr);
	*initrd_location_ptr=(uint32_t)PA2KA(mod_ptr->mod_start);
	*initrd_end_ptr=(uint32_t)PA2KA(mod_ptr->mod_end);
	printk("initrd image loaded at : %x " , *initrd_location_ptr);
	return 0;
	}
	else 
	{
	printk("failed to initialize initrd..\n");
	return 1;
	}
}
void initrd_test(multiboot_info_t * mboot_ptr , uint32_t initrd_location , uint32_t initrd_end)
{
	asm("xchg %bx,%bx");
	vfs_node_t * fs_root;
	uint32_t i= 0;
    	direntry_t *node = 0;
	printk("[*]initializing initrd..\n");
	fs_root = initialise_initrd(initrd_location);
 	while ( (node = readdir_fs(fs_root, i)) != 0)
   	 {
		printk("node [%d] ",i);
		asm("xchg %bx,%bx");
        	//printk("[initrd]: Found node ");
		//%s \n",node->name);
       		vfs_node_t *fsnode = finddir_fs(fs_root, node->name);
        	if ((fsnode->flags & 0x7) == FS_DIRECTORY)
        	{   
          	    printk("(directory) %s \n", node->name);
      	        }
        	else
        	{
		    asm("xchg %bx,%bx");
                    printk("contents:\n");
                    char * buf = (char *)kmalloc(32);
		    asm("xchg %bx,%bx");
                    uint32_t sz = read_fs(fsnode, 0, 32, buf);
	            printk("read_fs\n");
		    asm("xchg %bx,%bx");
            	    int j;
          	    for (j = 0; j < sz; j++)
                    printk("%s\n",buf+j);  
        	}
        	i++;
   	 }
	
}

void
bootup(uint32_t mboot_ptr){
	char ch,*pch;
	uint32_t * t;
	int i;
 	//for initrd
	multiboot_info_t * mboot_info_ptr;
	uint32_t initrd_location;
	uint32_t initrd_end;
	uint8_t fail;
	/***********************************
	 *** INITIATION OF BATCH USERMODE!!
	 *** THIS IS TEMPORARY
	 ***********************************/
	time_print();
	bios_init();
	scan_memory();
	mboot_info_ptr = (multiboot_info_t *)PA2KA(mboot_ptr);
	fail=initrd( mboot_info_ptr , &initrd_location , &initrd_end);
	//initrd_location= (uint32_t) PA2KA(initrd_location);
	asm("xchg %bx,%bx");
	processor_identify();
	lapic_init();
	i8254_calibrate_delay_loop();
	x86_setup_memory();
        //ap_init();
	mm_init();
	printk("haz we failed?");
	asm("xchg %bx,%bx");
	if(!fail)
	{
	initrd_test(mboot_info_ptr , initrd_location , initrd_end);
	}
	init_proc();
	kconsole_init();
	play();
}


void
play(){
	
	const char *buf;
	printk("Welcome to CatOS!\n");
	printk("For Commands type help\n");
	printk("This was developed by CATReloaded team: http://catreloaded.net\n\n");
	cga_set_attr(COLOR_RED|COLOR_GREEN);
//	sched_init();
	/*
	while(1){
		buf = readline("CatOS(@) ");
		if (buf!=NULL){
			printk("Command was %s\n",buf);			
		}

	}*/
	kconsole();
}

/** @} @}
 */
