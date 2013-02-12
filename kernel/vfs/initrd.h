 /** 
 * @file initrd.h
 * @author Menna Essa 
 * @brief basic VFS operations , calls file system implementation callback.
 * http://catreloaded.net
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
    uint32_t name[64];  
    uint32_t offset;   // Offset in the initrd that the file starts.
    uint32_t size;   
} initrd_file_header_t;

// Initialises the initial ramdisk. It gets passed the address of the multiboot module,
// and returns a completed filesystem node.
vfs_node_t *initialise_initrd(uint32_t location);

#endif
