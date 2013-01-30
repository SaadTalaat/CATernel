/**
 * @addtogroup MultiProcessors
 * @{
 * @file smp.c
 * @author Menna Essa
 * @brief SMP Support functions
 * @name Delay 
 * @{
 */ 

#include <types.h>
#include <arch/x86/mp/smp.h>
#include <arch/x86/mp/apic.h>
#include <arch/x86/bios/bios.h>
#include <arch/x86/mm/page.h>
#include <memvals.h>
//#include <arch/x86/mp/ap.h>

extern const char trampoline[];
extern const char trampoline_end[];
/**
 * @brief floating point structure
 */
fpstruct_t * fs;
/**
 * @brief configuration table.
 */
ct_hdr * ct;

//keep pointer to collect entries.
static ct_proc_entry * processor_entries;
ct_bus_entry * bus_entries;
ct_io_apic_entry * io_apic_entries;
ct_io_intr_entry *io_intr_entries;
ct_loc_intr_entry *loc_intr_entries;

//why do global variables freak out ? C trolling..
uint32_t processors_count;
uint32_t io_apic_cnt;
uint32_t bus_entry_cnt;
uint32_t io_apic_entry_cnt;
uint32_t io_intr_entry_cnt;
uint32_t loc_intr_entry_cnt;

/**
 * @brief checks if floating point structure is valid , the sum of all elements must be 0 
 * @param base base of table
 * @return 0 if true 1 if false
 */ 
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
/**
 * @brief checks if configuration is valid , the sum of all elements must be 0 
 * @param base base of table
 * @return 0 if true 1 if false
 */ 

uint8_t ct_check(void)
//<3 helenOS return 0 if succeeded 
{
	uint8_t *base = (uint8_t *) ct;
	uint8_t *ext = base + ct->base_table_len;
	uint8_t sum;
	uint16_t i;
	
	/*checksum for the base table */
	for (i = 0, sum = 0; i < ct->base_table_len; i++)
		sum = (uint8_t) (sum + base[i]);
	
	if (sum)
		return 1;
	
	/*checksum for the extended table */
	for (i = 0, sum = 0; i < ct->extnd_table_len; i++)
		sum = (uint8_t) (sum + ext[i]);
	
	if(sum) 
		return 1;
	else
		return 0;
}
/**
 * @brief parses the floating point structure
 * @param fs pointer to structure
 * @return none
 */ 
void fsp_print(fpstruct_t * fs)
{
	printk("Signature %x \n " , fs->Signature);
	printk("Configuration Table paddr %x \n", fs->config_addr);
	printk("Table Lenght %x \n" , fs->len);
	printk("Version %x \n" , fs->version);
	printk("Feature1 %x \n" , fs->feature1);
	printk("Feature2 %x \n" , fs->feature2);
}
/**
 * @brief parses the configuration table structure
 * @param none
 * @return none
 */ 
void ct_read_hdr(void)
{	
    
	printk("[*]Reading Configuration table.. \n");
	printk("ct Signature %x\n" , ct->Signature);
	printk("ct Base table Length %x\n" , ct->base_table_len);
	printk("ct Version %x\n" , ct->spec_ver);
	printk("ct oem_id %s\n" , ct->oem_id);
	printk("ct product id %s\n" , ct->product_id);
	printk("ct oem table pointer %p\n" , ct->oem_table_pointer);
	printk("ct oem table size %x\n" , ct->oem_table_size);
	printk("ct Entry count %x\n" , ct->entry_count);
	printk("ct L APIC address %x\n" , ct->lapic_addr);
	printk("ct Extended table length %x\n" , ct->extnd_table_len);
	printk("ct Extended table checksum %x\n" , ct->extnd_table_checksum);
	
}
/**
 * @brief parses the cpu entry
 * @param processor_entry pointer to ct_proc_entry structure
 * @return none
 */ 
void print_proc_entry(ct_proc_entry * processor_entry)
{
	printk("entry addr : %p \n" , processor_entry);
	printk("Entry type : %x \n" , processor_entry->entry_type);
	printk("LAPIC ID : %x \n" , processor_entry->lapic_id);
	printk("LAPIC Version : %x \n" , processor_entry->lapic_version);
	printk("CPU Flags : %x\n" , processor_entry->cpu_flags);
	printk("CPU Signature : %x \n" , processor_entry->cpu_signature);
	printk("Feature flags : %x \n", processor_entry->feature_flags);
}
/**
 * @brief parses the config table and sets the entries pointers
 * @param none
 * @return none
 */ 
void ct_entries(void)
{
	uintptr_t l_apic;

	processor_entries=NULL;
	bus_entries=NULL;
	io_apic_entries=NULL;
	io_intr_entries=NULL;
	loc_intr_entries=NULL;

        io_apic_cnt = 0;
	bus_entry_cnt = 0;
	io_apic_entry_cnt = 0;
	io_intr_entry_cnt = 0;
	loc_intr_entry_cnt = 0;
	processors_count=0U;
	if (ct->Signature != CT_SIGNATURE) {
		printk("[*]Bad Config table signature ; Aborting ..\n");
		return;
	}
	
	if (ct_check()) {
		printk("[*]Bad checksum , Config table maybe corrupted; Aborting ..\n");
		return;
	}
	l_apic = (uintptr_t) ct->lapic_addr;	
	uint8_t *cur = &ct->base_table[0];
	uint16_t i;
	for (i = 0; i < ct->entry_count; i++) {
		switch (*cur) {
		case PROCESSOR: 
			processor_entries = processor_entries ?
			processor_entries :(ct_proc_entry*) cur;		    
			processors_count++;
			printk("[*] Processor [%d] detected\n" , processors_count);
			//print_proc_entry((ct_proc_entry*) cur);
			//asm("xchg %bx,%bx");		
			cur += 20;
			break;
		case BUS:
			bus_entries = bus_entries ?
			    bus_entries : (ct_bus_entry *) cur;
			bus_entry_cnt++;
			printk("[*] Bus [%d] detected\n" , bus_entry_cnt);
			cur += 8;
			break;
		case IO_APIC: 
			io_apic_entries = io_apic_entries ?
			    io_apic_entries : (ct_io_apic_entry *) cur;
			io_apic_entry_cnt++;
			printk("[*] IO APIC [%d] detected\n" ,io_apic_entry_cnt);
			cur += 8;
			break;
		case IO_INTRPT_ASS:  
			io_intr_entries = io_intr_entries ?
			    io_intr_entries : (ct_io_intr_entry *) cur;
			io_intr_entry_cnt++;
	                printk("[*] IO INTERRUPT APIC [%d] detected\n" ,io_intr_entry_cnt);
			cur += 8;
			break;
		case LOC_INTRPT_ASS: 
			loc_intr_entries = loc_intr_entries ?
			    loc_intr_entries : (ct_loc_intr_entry *) cur;
			loc_intr_entry_cnt++;
	                printk("[*] IO INTERRUPT APIC [%d] detected\n" ,io_intr_entry_cnt);
			cur += 8;
			break;
		default:
			printk("[*] Bad CT Entry \n");
			return;
		}
		
	}
	//processors_count=processor_entry_cnt;
	//printk("[*] Detected total of %d processors ; Entries starting at %p \n" , processors_count , processor_entries);
	//asm("xchg %bx,%bx");
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
	//asm("xchg %bx,%bx");
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
	ct=(ct_hdr *)PA2KA((uint32_t)fs->config_addr);
	//asm("xchg %bx,%bx");
}
/**
 * @brief checks if cpu is bootstrap
 * @param processory_entry pointer to processor structure in config table
 * @return 1 if bootstrap 0 if not
 */ 
uint8_t cpu_is_bootstrap(ct_proc_entry * processor_entry)
{
	if((processor_entry->cpu_flags & 0x2) == 0x2)
		return 1;
	else
		return 0;
}
/**
 * @brief checks if cpu is enabled 
 * @param processory_entry pointer to processor structure in config table
 * @return 1 if enabled 0 if not
 */ 
uint8_t cpu_is_enabled(ct_proc_entry * processor_entry)
{
	if((processor_entry->cpu_flags & 0x1) == 0x1)
		return 1;
	else
		return 0;
}

/**
 * @brief Initializes application processors
 * @param void
 * @return void
 */ 
void map_AP_Startup(void)
{
	
	memcopy(TRAMPOLINE_START, trampoline, trampoline_end - trampoline);	
}
void ap_init(void)
{
	find_set_fps();
	if(fs==NULL) return;
	fsp_print(fs);
	printk("[*]Checking Configuration table integrity..");
	if(ct_check()==0)
		printk("Success!\n");
	else{ printk("Failed!\n");return; }  
	ct_read_hdr();
	ct_entries();
	//vector_test();
	uint32_t i;
	//printk("\n\nprocessor entry : %p \n" , processor_entries ) ;
	//asm("xchg %bx,%bx");
	uint8_t * processor_entry = (uint8_t *) processor_entries;
	ct_proc_entry * p;
	//map_AP_startup();
	for(i=0 ; i < processors_count ; i++ )
	{	
		printk("Attempting to boot up processor [%x] : addr %p \n" , i , processor_entry );
		//print_proc_entry(processor_entry);
		asm("xchg %bx,%bx");
		if(!cpu_is_enabled((ct_proc_entry *)processor_entry))
			{
				printk("Processor disabled , Skipping \n");
				processor_entry+=20;
				continue;
			}
		if(cpu_is_bootstrap((ct_proc_entry *)processor_entry))
			{
				printk("Boot strap already up , Skipping \n");	
				processor_entry+=20;
				continue;
			}
		p=(ct_proc_entry *)processor_entry;
	        apic_init_ipi(p->lapic_id);
		processor_entry+=20;

	}	
	
}
/**
 * @}
 * @}
 */




