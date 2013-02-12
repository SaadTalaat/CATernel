 /** 
 * @file vfs.c
 * @author Menna Essa 
 * @brief basic VFS operations , calls file system implementation callback.
 * http://catreloaded.net
 */

#include "vfs.h"
#include <types.h>
vfs_node_t *fs_root = 0; // The root of the filesystem.

//this is a stupid prototype , plain standards ; 
//to be edited when fs implementations come to life
uint32_t read_fs(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    if (node->read != 0)
        return node->read(node, offset, size, buffer);
    else
        return 0;
}

uint32_t write_fs(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    if (node->write != 0)
        return node->write(node, offset, size, buffer);
    else
        return 0;
}

void open_fs(vfs_node_t *node, uint8_t read, uint8_t write)
{
    if (node->open != 0)
        return node->open(node);

}

void close_fs(vfs_node_t *node)
{
    if (node->close != 0)
        return node->close(node);
}

direntry_t* readdir_fs(vfs_node_t *node, uint32_t index)
{
    // Is the node a directory
    if ( (node->flags&0x7) == FS_DIRECTORY &&
         node->readdir != 0 )
        return node->readdir(node, index);
    else
        return 0;
}

vfs_node_t* finddir_fs(vfs_node_t *node, char *name)
{
    if ( (node->flags&0x7) == FS_DIRECTORY &&  
	  node->finddir != 0 ) 
          return node->finddir(node, name);
    else
        return 0;
}
