#ifndef _CATERNEL_EXT2FS_H_
#define _CATERNEL_EXT2FS_H_
#include <types.h>
/*
 * Constants relative to the data blocks
 */
#define	EXT2_NDIR_BLOCKS		12
#define	EXT2_IND_BLOCK			EXT2_NDIR_BLOCKS
#define	EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)
#define	EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)
#define	EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)
#define EXT2_INOSZ_V0 128
#define EXT2_NAME_LEN 255
/* File System states */
#define EXT2_CLEAN 	1
#define EXT2_ERR0R 	2
/*
 * Special inodes numbers 
 */
#define	EXT2_BAD_INO		 1	/* Bad blocks inode */
#define EXT2_ROOT_INO		 2	/* Root inode */
#define EXT2_ACL_IDX_INO	 3	/* ACL inode */
#define EXT2_ACL_DATA_INO	 4	/* ACL inode */
#define EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#define EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#define EXT2_FIRST_INO		11	/* First non reserved inode */

/*
 * The second extended file system magic number
 */
#define EXT2_PRE_02B_MAGIC	0xEF51
#define EXT2_SUPER_MAGIC	0xEF53

/*
 * Maximal count of links to a file
 */
#define EXT2_LINK_MAX		32000

/*Error handling methods */
#define EXT2_IGNORE	1
#define EXT2_REMOUNT_RO 2
#define EXT2_KPANIC	3
/*OS IDs*/
#define EXT2_OS_LINUX	0
#define EXT2_OS_HURD	1
#define EXT2_OS_MASIX	2
#define EXT2_OS_FBSD	3
#define EXT2_OS_LITES	4

/*
 * Inode Type and permission
 */
#define EXT2_ROOT_INODE	 2	/* Root inode */
/*(bits 15 to 12) of the i_mode 16-bit field */
#define I_FIFO		 0x1000
#define I_CHAR_DEVICE 	 0x2000
#define I_DIRECTORY 	 0x4000
#define I_BLOCK_DEVICE   0x6000
#define I_REG_FILE	 0x8000
#define I_SYM_LINK 	 0xA000
#define I_UNIX_SOCK 	 0xC000
/*Permissions occupy the bottom 12 bits of the i_mode 16-bit field */
//O -> Other G-> Groupt U->User 
#define I_OEXEC_P	0X001
#define I_OWRITE_P	0x002
#define I_OREAD_P	0x004
#define I_GEXEC_P	0x008
#define I_GWRITE_P	0x010
#define I_GREAD_P	0x020
#define I_UEXEC_P	0x040
#define I_UWRITE_P	0x080
#define I_UREAD_P	0x100
#define I_STICKY 	0x200
#define I_SET_GID 	0x400
#define I_SET_UID 	0x800

/**Inode flags **/	
#define I_SEC_DEL 		0x00000001  /*Secure deletion*/
#define	I_KEEP_CPY		0x00000002  /*Keep a copy of data when deleted*/
#define	I_FILE_COMPRESSION	0x00000004  /*File compression*/
#define	I_SYNC_UPDATES		0x00000008  /*Synchronous updates*/
#define	I_IMMUTABLE_FILE	0x00000010  /*Immutable file*/
#define	I_APPEND_ONLY		0x00000020  /* Append only */
#define	I_NO_DUMP		0x00000040  /* File is not included in 'dump' command */
#define	I_NO_LAT		0x00000080  /* Last accessed time should not updated */
#define	I_HASH_INDEXED_DIR	0x00010000  /* Hash indexed directory */
#define I_AFS_DIR		0x00020000  /* AFS directory */
#define	I_JOURNAL_FILE_DATA	0x00040000  /* Journal file data */

/**Directory Entry Types **/
#define I_DIR_UNKOWN 	0x0
#define	I_DIR_REGULAR_F 0x1
#define	I_DIR_DIRECTORY 0x2
#define	I_DIR_CHAR_DEV 	0x3
#define I_DIR_BLOCK_DEV 0x4
#define I_DIR_FIFO	0x5
#define	I_DIR_SOCK	0x6
#define I_DIR_SYM_LINK  0x7
/**Features **/
//Optional feature flags, useless atm
#define EXT2_PRELOC	0x0001
#define EXT2_AFS_INO	0x0002
#define EXT2_JRNL	0x0004
#define	EXT2_INO_EXTNDATTR 0x0008
#define EXT2_FS_RESIZE	0x0010
#define	EXT2_DIR_HASHI	0x0020
//Required Features flags 
#define EXT2_COMPRESSION 	0x0001
#define EXT2_DIR_TYPE_F		0x0002
#define EXT2_FS_REPLAY_JOR	0x0004
#define EXT2_FS_JOR_DEV		0x0008
//Read only features flags
#define EXT2_SPARSE		0x0001
#define EXT2_64FZ		0x0002
#define EXT2_DIR_BIN_TREE 	0x0004




//Base Super block

struct ext2_super_block {
	uint32_t  s_inodes_count;	/* Inodes count */
	uint32_t  s_blocks_count;	/* Blocks count */
	uint32_t  s_r_blocks_count;     /* Reserved blocks count */
	uint32_t  s_free_blocks_count;   /* Free blocks count */
	uint32_t  s_free_inodes_count;  /* Free inodes count */
	uint32_t  s_first_data_block;   /* First Data Block */
	uint32_t  s_log_block_size;     /* Block size */
	uint32_t  s_log_frag_size;	/* Fragment size */
	uint32_t  s_blocks_per_group;   /* # Blocks per group */
	uint32_t  s_frags_per_group;    /* # Fragments per group */
	uint32_t  s_inodes_per_group;   /* # Inodes per group */
	uint32_t  s_mtime;		/* Mount time */
	uint32_t  s_wtime;		/* Write time */
	uint16_t  s_mnt_count;	        /* Mount count */
	uint16_t  s_max_mnt_count;	/* Maximal mount count */
	uint16_t  s_magic;		/*(0xef53) Magic signature */
	uint16_t  s_state;		/* File system state */
	uint16_t  s_errors;	       /* Behaviour when detecting errors */
	uint16_t  s_min_pad;	       /* Minor portion of version*/
	uint32_t  s_lastcheck;	       /* time of last check */
	uint32_t  s_checkinterval;     /* max. time between checks */
	uint32_t  s_os_id;	       /*ID from which the filesystem on this volume was created*/
	uint32_t  s_maj_pad;	       /*Major portion of version*/
	uint16_t  s_uid;		/* User ID that can use reserved blocks */	
	uint16_t  s_gid;		/* Group ID that can use reserved blocks */
	//Extended super block features 

        uint32_t  s_first_ino;            /* First non-reserved inode */
    	uint16_t  s_inode_size;           /* size of inode structure */
        uint16_t  s_block_group_nr;       /* block group # of this superblock */
    	uint32_t  s_feature_compat;       /* compatible feature set */
    	uint32_t  s_feature_incompat;     /* incompatible feature set */
    	uint32_t  s_feature_ro_compat;    /* readonly-compatible feature set */
    	uint8_t   s_uuid[16];             /* 128-bit uuid for volume */
    	char       s_volume_name[16];      /* volume name */
    	char       s_last_mounted[64];     /* directory where last mounted */
    	uint32_t   s_algorithm_usage_bitmap; /* For compression */
	uint8_t    s_prealloc_blocks;      /* # of blocks to try to preallocate*/
   	uint8_t    s_prealloc_dir_blocks;  /* # to preallocate for dirs */
    	uint16_t   s_padding1;
	uint8_t    s_journal_uuid[16];     /* uuid of journal superblock */
    	uint32_t   s_journal_inum;         /* inode number of journal file */
    	uint32_t   s_journal_dev;          /* device number of journal file */
        uint32_t   s_last_orphan;          /* start of list of inodes to delete */
	uint32_t   s_reserved[197];	   /* Padding to the end of the block */
	
};	

//Directory entry , from linux 1.0
struct ext2_dir_entry {
	uint32_t inode;			/* Inode number */
	uint16_t rec_len;			/* Directory entry length */
	uint8_t  name_len;			/* Name length least sign. 8-bits*/
	uint8_t  type				/*Type indicator (only if the feature bit for 
						directory entries have file type byte" is set, 
						else this is the most-significant 
						8 bits of the Name Length) */
	int8_t name[EXT2_NAME_LEN];	/* File name */
};

//Block group descriptor
struct ext2_group_desc
{
	uint32_t bg_block_bitmap;		/*  Block address of block usage bitmap */
	uint32_t bg_inode_bitmap;		/*  Block address of inode usage bitmap  */
	uint32_t bg_inode_table;		/* address ofInodes table block */
	uint16_t bg_free_blocks_count;	/* Free blocks count */
	uint16_t bg_free_inodes_count;	/* Free inodes count */
	uint16_t bg_used_dirs_count;	/* Directories count */
	uint16_t bg_pad;
	uint32_t  bg_reserved[3];
};

//Inode structure
struct ext2_inode {
	uint16_t 	i_mode;		/* Type and permissions */
	uint16_t 	i_uid;		/*  Uid */
	uint32_t 	i_size;		/* lower 32 Size in bytes */
	uint32_t 	i_atime;	/* Access time */
	uint32_t  	i_ctime;	/* Creation time */
	uint32_t 	i_mtime;	/* Modification time */
	uint32_t	i_dtime;	/* Deletion Time */
	uint16_t 	i_gid;		/* Group Id */
	uint16_t 	i_links_count;	/* hard links count */
	uint32_t 	i_blocks;	/* Count of disk sectors (not Ext2 blocks) in use by this inode */
	uint32_t 	i_flags;	/* File flags */
	uint32_t  	i_reserved1;
	uint32_t  	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
	uint32_t  	i_version;	/* Generation Number(for NFS) */
	uint32_t  	i_file_acl;	/* File ACL , reserved in v0 */
	uint32_t  	i_dir_acl;	/* Directory ACL , reserved in v0 */
	uint32_t  	i_faddr;	/* block address of fragment address */
	/**OS SPECIFIC VALUES from minix inode.h**/		
	    union {
        struct {
            uint8_t      l_i_frag;       /* Fragment number */
            uint8_t      l_i_fsize;      /* Fragment size */
            uint16_t     l_i_reserved1;
            uint16_t     l_i_uid_high;   /* High 16 bits of 32-bit User ID */
            uint16_t     l_i_gid_high;   /* High 16 bits of 32-bit Group ID*/
            uint32_t     l_i_reserved2;
        } linux;
        struct {
            uint8_t   h_i_frag;       /* Fragment number */
            uint8_t   h_i_fsize;      /* Fragment size */
            uint16_t  h_i_mode_high;  /* High 16 bits of 32-bit "Type and Permissions" field */
            uint16_t  h_i_uid_high;   /* High 16 bits of 32-bit User ID*/
            uint16_t  h_i_gid_high;   /* High 16 bits of 32-bit Group I */
            uint32_t  h_i_author;     /* User ID of author*/
        } hurd2;
        struct {
            uint8_t    m_i_frag;       /* Fragment number */
            uint8_t    m_i_fsize;      /* Fragment size */
            uint16_t   m_pad1;
            uint32_t   m_i_reserved2[2];
        } masix2;
    } osd2;                         /* OS dependent 2 */
};




#endif
