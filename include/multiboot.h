/**
 * @file multiboot.h
 * @author Menna Essa
 * @date 19 November, 2012
 * @brief Multiboot specifications 
 *
 */

#ifndef _CATERNEL_MB_H_
#define _CATERNEL_MB_H_

#include <types.h>
#define MULTIBOOT_SEARCH                   8192
#define MULTIBOOT_HEADER_MAGIC             0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC         0x2BADB002
#define MULTIBOOT_UNSUPPORTED              0x0000fffc
#define MULTIBOOT_AOUTF_FLAG_KLUDGE        0x00010000
#define MULTIBOOT_FLAG_MEM     0x001
#define MULTIBOOT_FLAG_DEVICE  0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS    0x008
#define MULTIBOOT_FLAG_AOUT    0x010
#define MULTIBOOT_FLAG_ELF     0x020
#define MULTIBOOT_FLAG_MMAP    0x040
#define MULTIBOOT_FLAG_CONFIG  0x080
#define MULTIBOOT_FLAG_LOADER  0x100
#define MULTIBOOT_FLAG_APM     0x200
#define MULTIBOOT_FLAG_VBE     0x400

    
 /* Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN      0x00001000
 /* Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN     0x00000004

    
typedef struct 
     {
     uint32_t magic;  
     uint32_t flags;    
     /* The above fields plus this one must equal 0 mod 2^32. */
     uint32_t checksum; 
     /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
     uint32_t header_addr;
     uint32_t load_addr;
     uint32_t load_end_addr;
     uint32_t bss_end_addr;
     uint32_t entry_addr;
     
       /* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
     uint32_t mode_type;
     uint32_t width;
     uint32_t height;
     uint32_t depth;
     } __attribute__((packed))  multiboot_header_t;
     
/* The section header table for ELF. */
typedef struct 
     {
       uint32_t num;
       uint32_t size;
       uint32_t addr;
       uint32_t shndx;
     }__attribute__((packed)) multiboot_elf_section_header_t;

typedef struct 
     {
       uint32_t tabsize;
       uint32_t strsize;
       uint32_t addr;
       uint32_t reserved;
     }__attribute__((packed)) multiboot_aout_symbol_table_t;
     

typedef struct {

	uint32_t moduleStart;
	uint32_t moduleEnd;
	char     string[8];

}__attribute__((packed)) multiboot_module_entry_t;
typedef struct 
     {
       /* Multiboot info version number */
       uint32_t flags;    
       /* Available memory from BIOS */
       uint32_t mem_lower;
       uint32_t mem_upper;     
       /* "root" partition */
       uint32_t boot_device;   
       /* Kernel command line */
       uint32_t cmdline;
       /* Boot-Module list */
       uint32_t mods_count;
       uint32_t mods_addr;  
       union
       {
         multiboot_aout_symbol_table_t aout_sym;
         multiboot_elf_section_header_t elf_sec;
       } u;
       /* Memory Mapping buffer */
       uint32_t mmap_length;
       uint32_t mmap_addr;
       /* Drive Info buffer */
       uint32_t drives_length;
       uint32_t drives_addr;     
       /* ROM configuration table */
       uint32_t config_table;     
       /* Boot Loader Name */
      uint32_t boot_loader_name;     
       /* APM table */
       uint32_t apm_table;   
       /* Video */
       uint32_t vbe_control_info;
       uint32_t vbe_mode_info;
       uint16_t vbe_mode;
       uint16_t vbe_interface_seg;
       uint16_t vbe_interface_off;
       uint16_t vbe_interface_len;
     }__attribute((packed)) multiboot_info_t ;
     
typedef struct 
     {
       uint32_t size;
       uint64_t addr;
       uint64_t len;
     #define MULTIBOOT_MEMORY_AVAILABLE              1
     #define MULTIBOOT_MEMORY_RESERVED               2
       uint32_t type;
     } __attribute__((packed)) mutiboot_mmap_t;

typedef struct
     {
       /* the memory used goes from bytes 'mod_start' to 'mod_end-1'*/
       uint32_t mod_start;
       uint32_t mod_end; 
       /* Module command line */
       uint32_t cmdline;  
       /* padding to take it to 16 bytes (must be zero) */
       uint32_t pad;
     }__attribute__((packed)) multiboot_mod_t;

#endif
