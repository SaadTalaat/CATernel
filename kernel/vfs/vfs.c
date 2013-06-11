/**
 * @addtogroup VFS
 * @{
 * @file vfs/vfs.c
 * @author Menna Essa
 * @brief Virtual file system standards , In order to abstract different file system calls.
 * @name Initrd
 * @{
 */
#include "vfs.h"
#include "initrd.h"
#include <types.h>
vfs_node_t *fs_root = 0; // The root of the filesystem.

//plain standards ; 
//to be edited when fs implementations come to life

/**
 * @brief Calls the node's read operation , saved as read in the node structure.
 * @param vfs_node_t *node the file's node to be read.
 * @param uint32_t offset where to start reading ( 0 for beggining of the file).
 * @param uint32_t size  size of data to read.
 * @param uint8_t *buffer buffer to be read into.
 * @return uint32_t : 0 if the node does not support read operation else The result of the read operation.
 */

uint32_t read_fs(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{

    if (node->read != 0)
        return node->read(node, offset, size, buffer);
    else
        return 0;
}

/**
 * @brief Calls the node's write operation , saved as write in the node structure.
 * @param vfs_node_t *node the file's node to be written into.
 * @param uint32_t offset where to start write ( 0 for beggining of the file).
 * @param uint32_t size  size of data to write.
 * @param uint8_t *buffer buffer containing data to write.
 * @return uint32_t : 0 if the node does not support open operation else The result of the write operation.
 */

uint32_t write_fs(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    if (node->write != 0)
        return node->write(node, offset, size, buffer);
    else
        return 0;
}

/**
 * @brief Calls the node's open operation , saved as write in the node structure.
 * @param vfs_node_t *node the file's node to open.
 * @param uint8_t read  read flag.
 * @param uint8_t write write flag
 * @return uint32_t : 0 if the node does not support open operation else The result of the open operation.
 */
uint8_t open_fs(vfs_node_t *node, uint8_t read, uint8_t write)
{
    if (node->open != 0)
        return node->open(node , read , write);
    else return 0;
	

}

/**
 * @brief Calls the node's open operation , saved as write in the node structure.
 * @param vfs_node_t *node the file's node to open.
 * @return uint32_t : 0 if the node does not support open operation else The result of the close operation.
 */
uint8_t close_fs(vfs_node_t *node)
{
    if (node->close != 0)
        return node->close(node);
   else
	return 0;
}

/**
 * @brief Calls the node's read directory operation , saved as readdir in the node structure.
 * @param vfs_node_t *node the directory node to be read.
 * @param uint32_t directory index.
 * @return direntry_t * : 0 if the node does not support open operation else The directory entry pointer returned
		 	by the readdir operation
 */
direntry_t* readdir_fs(vfs_node_t *node, uint32_t index)
{
    // Is the node a directory
    if ( (node->flags&0x7) == FS_DIRECTORY &&
         node->readdir != 0 )
        return node->readdir(node, index);
    else
        return 0;
}


/**
 * @brief Calls the node's find directory operation , save as finddir in the node strucutre.
 * @param vfs_node_t *node head to start from.
 * @param uint32_t char * name name of the directory to search for.
 * @return uint32_t : pointer to the node if succeeded , 0 if failed.
 */ 
vfs_node_t* finddir_fs(vfs_node_t *node, char *name)
{
    if ( (node->flags&0x7) == FS_DIRECTORY &&  
	  node->finddir != 0 ) 
          return node->finddir(node, name);
    else
        return 0;
}



/**
 * @}
 * @}
 */
