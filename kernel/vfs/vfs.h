 /** 
 * @file vfs.h
 * @author Menna Essa 
 * @brief basic VFS Structures , operations definitions.
 * http://catreloaded.net
 */

#include <types.h>
#ifndef _CATERNEL_VFS_H_
#define _CATERNEL_VFS_H_

//fs types
#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 // Is the file an active mountpoint?

struct vfs_node;
//POSIX Standard
typedef uint32_t (*read_type_t)( struct vfs_node *,uint32_t,uint32_t,uint8_t*);
typedef uint32_t (*write_type_t)(struct vfs_node *,uint32_t,uint32_t,uint8_t*);
typedef void (*open_type_t)(struct vfs_node*);
typedef void (*close_type_t)(struct vfs_node *);
typedef struct {
  char name[128]; 
  uint32_t inode_n; 
}direntry_t; 

typedef direntry_t * (*readdir_type_t)(struct vfs_node *,uint32_t);
typedef struct vfs_node * (*finddir_type_t)(struct vfs_node*,char *name);

typedef struct vfs_node
{
   char name[128]; 
   uint32_t inode;       // This is device-specific - provides a way for a filesystem to identify files.    
   uint32_t mask;        //permissions mask.
   uint32_t uid;        
   uint32_t gid;         
   uint32_t flags;       //the node type (file , dir , etc).
   uint32_t size;        // Size of the file, in bytes.
   uint32_t impl;        //impl dependent;the driver can set to track which filesystem instance it belongs to .
   read_type_t read;
   write_type_t write;
   open_type_t open;
   close_type_t close;
   readdir_type_t readdir;
   finddir_type_t finddir;
   struct fs_node *ptr; // Used by mountpoints and symlinks.
} vfs_node_t; 

//typedef vfs_node_t * (*finddir_type_t)(vfs_node_t*,char *name); 



// The root of the filesystem.
extern vfs_node_t *fs_root; 
// Standard read/write/open/close functions.
uint32_t read_fs(vfs_node_t *node, uint32_t  offset, uint32_t size, uint8_t  *buffer);
uint32_t write_fs(vfs_node_t *node, uint32_t  offset, uint32_t size, uint8_t *buffer);
void open_fs(vfs_node_t *node, uint8_t read, uint8_t write);
void close_fs(vfs_node_t *node);
direntry_t *readdir_fs(vfs_node_t *node, uint32_t  index);
vfs_node_t *finddir_fs(vfs_node_t *node, char *name);

#endif
