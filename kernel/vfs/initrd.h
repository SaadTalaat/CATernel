/**
 * @addtogroup VFS
 * @{
 * @file vfs/initrd.h
 * @author Menna Essa
 * @brief Initrd file system headers
 * @name Initrd
 * @{
 */

#ifndef CATERNEL_INITRD_H
#define CATERNEL_INITRD_H

#include <types.h>
#include <arch/x86/x86.h>
#include <arch/x86/elf.h>
#include "vfs.h"


typedef struct
{
    uint32_t nfiles; // The number of files in the ramdisk.
} initrd_header_t;

typedef struct
{
    uint8_t magic;       //for error checking.
    char name[64];  
    uint32_t offset;   // Offset in the initrd that the file starts.
    uint32_t size;   
} initrd_file_header_t;
      

/**
 * @}
 * @}
 */      
// Initialises the initial ramdisk. pass the address of the multiboot module,
// and returns a completed filesystem node.
vfs_node_t *initialise_initrd(uint32_t location);
uint32_t initrd_read(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
#endif
