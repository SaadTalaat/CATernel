/**
 * @addtogroup VFS
 * @{
 * @file vfs/initrd.c
 * @author Menna Essa
 * @brief Initrd file system operations specifications and implementation
 * @name Initrd
 * @{
 */

#include <mm/mm.h>
#include "initrd.h"

static initrd_header_t *initrd_header=NULL;     
static initrd_file_header_t *file_headers=NULL; 
static vfs_node_t *initrd_root=NULL;             
static vfs_node_t *initrd_dev=NULL;             
static vfs_node_t *root_nodes=NULL;  
int nroot_nodes;                    
direntry_t dirent;


/**
 * @brief Read operation implementation , reads the offset from the node structure , Reads
	  data of size specified by the call ; after checking that the file is legal.
 * @param vfs_node_t *node the file's node to be read.
 * @param uint32_t offset where to start reading ( 0 for beggining of the file).
 * @param uint32_t size  size of data to read.
 * @param uint8_t *buffer buffer to be read into.
 * @return uint32_t : 0 is failed , size of data read if succeeded .
 */
uint32_t initrd_read(vfs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    initrd_file_header_t * hdr=	file_headers;
    initrd_file_header_t header = hdr[node->inode];
    if (offset > header.size)
        return 0;
    if (offset+size > header.size)
        size = header.size-offset;
    	memcopy(buffer, (uint8_t*) (header.offset+offset), size);
    return size;
}




/**
 * @brief Directory traversal , reads next directory ; still immature
 * @param vfs_node_t *node the directory node to be read.
 * @param uint32_t directory index.
 * @return uint32_t : pointer to directory entry if succeeded , 0 if failed.
 */ 

static  direntry_t * initrd_readdir(vfs_node_t *node, uint32_t index)
{
    if (node == initrd_root && index == 0)
    {
      memcopy(dirent.name, "dev\0" , 4);
      dirent.inode_n = 0;
      return &dirent;
    }

    if (index-1 >= nroot_nodes){
        return 0;
	}
    memcopy(dirent.name, root_nodes[index-1].name,126); //I need strcpy..
    memcopy(dirent.name+126 , "\0",1);
    dirent.inode_n = root_nodes[index-1].inode;
    return &dirent;
}

/**
 * @brief Directory traversal , searches for a direcotry , very Immature.
 * @param vfs_node_t *node head to start from.
 * @param uint32_t char * name name of the directory to search for.
 * @return uint32_t : pointer to directory entry if succeeded , 0 if failed.
 */ 
static vfs_node_t *initrd_finddir(vfs_node_t *node, char *name)
{
    if (node == initrd_root &&
        name[0]=='d' && name[1]=='e' && name[2]=='v')//strcmp !
        return initrd_dev;
	/*TODO , Continue traversal when i get strcmp!*/
}


/**
 * @brief Initializes file systems structures (root directory , /dev direcotry and file nodes)
		      to be able to preform files system operations.
 * @param uint32_t location : pointer to the start of the initrd image.
 * @return vfs_node_t* initrd filesystem root node.
 */ 

vfs_node_t *initialise_initrd(uint32_t location)//from the multiboot_info structure
{
    // Initialise the main and file header pointers and populate the root directory.
    initrd_header = (initrd_header_t *)location;
    file_headers = (initrd_file_header_t *) (location+sizeof(initrd_header_t));
    // Initialise the root directory.
    initrd_root = (vfs_node_t*)kmalloc(sizeof(vfs_node_t));
    memcopy(initrd_root->name, "initrd\0",7);
    initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->size = 0;
    initrd_root->flags = FS_DIRECTORY;
    initrd_root->read = 0;
    initrd_root->write = 0;
    initrd_root->open = 0;
    initrd_root->close = 0;
    initrd_root->readdir = &initrd_readdir;
    initrd_root->finddir = &initrd_finddir;
    initrd_root->ptr = 0;
    initrd_root->impl = 0;
    // Initialise the /dev directory
    initrd_dev = (vfs_node_t*)kmalloc(sizeof(vfs_node_t));
    memcopy(initrd_dev->name, "dev\0",4);
    initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->size = 0;
    initrd_dev->flags = FS_DIRECTORY;
    initrd_dev->read = 0;
    initrd_dev->write = 0;
    initrd_dev->open = 0;
    initrd_dev->close = 0;
    initrd_dev->readdir = &initrd_readdir;
    initrd_dev->finddir = &initrd_finddir;
    initrd_dev->ptr = 0;
    initrd_dev->impl = 0;
    root_nodes = (vfs_node_t*)kmalloc(sizeof(vfs_node_t) * initrd_header->nfiles);
    nroot_nodes = initrd_header->nfiles;
    // For every file...
    int i;
    for (i = 0; i < initrd_header->nfiles; i++)
    {
        // Edit the file's header - currently it holds the file offset
        // relative to the start of the ramdisk. We want it relative to the start
        // of memory.
        file_headers[i].offset += location;
        // Create a new file node.
        memcopy(root_nodes[i].name, file_headers[i].name,126);
	memcopy(root_nodes[i].name+126,"\0",1);
	//printk("file name: %s \n" , file_headers[i].name);
	//printk("offset at %x \n", file_headers[i].offset);
	//printk("file contents: %s \n" , file_headers[i].offset);
	//asm("xchg %bx,%bx");
        root_nodes[i].read = &initrd_read;
        root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
        root_nodes[i].size = file_headers[i].size;
        root_nodes[i].inode = i;
        root_nodes[i].flags = FS_FILE;     
        root_nodes[i].write = 0;
        root_nodes[i].readdir = 0;
        root_nodes[i].finddir = 0;
        root_nodes[i].open = 0;
        root_nodes[i].close = 0;
        root_nodes[i].impl = 0;
    }
    return initrd_root;
}

/**
 * @}
 * @}
 */
