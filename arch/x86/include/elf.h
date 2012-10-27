/**
 * @addtogroup x86-boot
 * @{
 */

/**
 * @file elf.h
 * @author Saad Talaat 
 * @date 22/10/2011
 * @brief Elf binary structures and constants, for parsing binaries.
 *
 */

#ifndef _CATERNEL_ELF_H
#define _CATERNEL_ELF_H
/* Start of Magic Definitions */
/**
 * @name Elf-Member-Constants
 * @{
 */
#define ELF_MAGIC 0x464C457F
#define MAGIC_LEN 16

#define M_CLASS_OFF 4	//File Class offset
#define M_CLASSNONE 0	//Invalid Class
#define M_CLASS32 1	//32-bit Objects
#define M_CLASS64 2	//64-bit Objects
#define M_CLASSNUM 3

#define M_DATA_OFF 5	//Data encoding byte offset
#define	M_DATANONE 0	//Invalid Data encoding
#define M_DATA2LE	1	// 2's complement Little endian
#define M_DATA2BE 2	//2's complement Big endian
#define M_DATANUM 3

#define M_VERSION 6	//File version offset

#define M_OSABI 7		//OS/ABI offset
#define M_OSABI_SYSV 0	//Unix System V
#define M_OSABI_HPUX 1	//HP-UX

#define M_ABIVERSION 8 	//ABI version offset
#define	M_ELF_PADDING 9	//Padding bytes offset

/*
 * File types
 */
#define T_TYPE_NONE 0
#define T_TYPE_REL 1
#define T_TYPE_EXEC 2
#define T_TYPE_DYN 3
#define T_TYPE_CORE 4
#define T_TYPE_LOPROC 0xff00
#define T_TYPE_HIPROC 0xffff

/*
 * Machine types "since i will only work in i386 i will def one value"
 */
#define M_MACHINE_I386	3	//intel Machine

/*
 * Version types 
 */
#define V_VERSION_NONE 0
#define V_VERSION_CURRENT 1
#define V_VERSION_NUM 2

/*
 * Program header permissions Flags
 */
#define P_PROGHDR_R 0x4
#define P_PROGHDR_W 0x2
#define P_PROGHDR_E 0x1

/**
 * @}
 */
/**
 * @name Elf-structures
 * @{
 */


/**
 * @struct elfhdr
 * @brief Elf binary header values
 */
typedef struct elfhdr{
	/** the elf magic number */
	uint8_t magic[MAGIC_LEN];
	/** File type (EXEC/RELOC..etc) */
	uint16_t type;
	/** Machine type */
	uint16_t machine;
	/** Version */
	uint32_t version;
	/** Address of entry point */
	uint32_t entry;	
	/** offset of program headers */
	uint32_t phroff;
	/** offset of Section headers */
	uint32_t shroff;
	/** Flags */
	uint32_t flags;
	/** Elf header size */
	uint16_t ehsize;
	/** Program header size */
	uint16_t phrsize;
	/** Number of program headers */
	uint16_t phrnum;
	/** Section header size */
	uint16_t shrsize;
	/** Number of section headers */
	uint16_t shrnum;
	/** Section header string table */
	uint16_t shrstrtbl;
} elfhdr;

/**
 * @struct proghdr
 * @brief An ELF program header structure.
 */
typedef struct proghdr{
	uint32_t type;
	uint32_t offset;
	uint32_t vaddr;
	uint32_t paddr;
	uint32_t filesz;
	uint32_t memsz;
	uint32_t flags;
	uint32_t align;
} prohdr;

/**
 * @struct sechdr
 * @brief An ELF section header structure.
 */
typedef struct sechdr{
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t addralign;
	uint32_t entsize;

} secthdr;
/** @} */
#endif
/**
 *@}
 */
