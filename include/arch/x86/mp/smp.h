/**
 * @file smp.h
 * @author Saad Talaat
 * @date 19 November, 2012
 * @brief Multiprocessor support header
 *
 */

#ifndef _CATERNEL_X86_MP_SMP_H_
#define _CATERNEL_X86_MP_SMP_H_
#define NCPUS 4
#define FS_SIGNATURE  0x5f504d5f // _MP_ Signature
#define CT_SIGNATURE  0x504d4350 // PCMP Singature 
/*BASE MP CONFIGURATION ENTRY TYPES*/ 
#define PROCESSOR 	0
#define BUS 	  	1
#define IO_APIC   	2
#define IO_INTRPT_ASS   3
#define LOC_INTRPT_ASS  4 
/*Processors Signatures*/
#define INVALID		000000000000
#define INTEL486DX  	010000000001
#define INTEL486SX	010000100000
//will continue listing them later.

/*Feature flags*/
//MSP PAGE 45 later later

/*Interrupt types , used with ct_io_intr_entry*/
#define INT 	0
#define NMI 	1
#define SMI 	2
#define EXTINT  3

typedef struct {
  	uint32_t Signature;
	uintptr_t config_addr;
	uint8_t len;
	uint8_t version;
	uint8_t checksum;
	uint8_t feature1;
	uint8_t feature2;
	//feature 3:5 is reserved =0 : ignore!
		
}__attribute__((packed)) fpstruct_t;

typedef struct {
	uint32_t Signature;
	uint16_t base_table_len;
	uint8_t	 spec_ver;
	uint8_t  checksum;
	uint8_t oem_id[8];
	uint8_t product_id[12];
	uintptr_t  oem_table_pointer;
	uint16_t oem_table_size;	
	uint16_t entry_count;
	uint32_t lapic_addr;
	uint16_t extnd_table_len;
	uint8_t  extnd_table_checksum;
	//is intel trolling ?
	uint8_t reserved;
	uint8_t base_table[0];
;	
}__attribute__((packed)) ct_hdr;

typedef struct{
	uint8_t entry_type; 
	uint8_t lapic_id;   
	uint8_t lapic_version; 
	uint8_t cpu_flags;
	//unsigned feature_enabled:1;   
	//unsigned feature_bootstrap:1; 
	//unsigned features_pad:6;     
	uint8_t cpu_signature[4];
	uint32_t feature_flags; 
}__attribute__((packed)) ct_proc_entry;

typedef struct {
	uint8_t entry_type;
	uint8_t bus_id;
	uint8_t bus_type[6];
} __attribute__ ((packed)) ct_bus_entry;


typedef struct{
	uint8_t entry_type;
	uint8_t io_apic_id;
	uint8_t io_apic_ver;
	uint8_t io_apic_flags;
	uint8_t io_apic_addr;
}__attribute__((packed)) ct_io_apic_entry;

typedef struct {
	uint8_t type;
	uint8_t intrrupt_type;
	uint8_t po_el;
	uint8_t pad;
	uint8_t src_bus_id;
	uint8_t src_bus_irq;
	uint8_t dst_io_apic_id;
	uint8_t dst_io_apic_pin;
} __attribute__ ((packed)) ct_io_intr_entry;

typedef struct {
	uint8_t type;
	uint8_t intrrupt_type;
	uint8_t po_el;
	uint8_t pad;
	uint8_t src_bus_id;
	uint8_t src_bus_irq;
	uint8_t dst_loc_apic_id;
	uint8_t dst_loc_apic_pin;
} __attribute__ ((packed)) ct_loc_intr_entry;


extern fpstruct_t * fs;
extern  ct_hdr * ct;

uint8_t fps_check(uint8_t *base);
uint8_t ct_check(void);
void fsp_print(fpstruct_t * fs);
void ct_read_hdr(void);
static void ct_entries(void);
void find_set_fps(void);

#endif
