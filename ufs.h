#ifndef UFS_H
#define UFS_H

#define UFS_BLOCK_SIZE (4096)
#define DIRECT_PTRS (30)
#define INDIRECT_PTRS (1)
#define MFS_DIRECTORY (0)
#define MFS_REGULAR_FILE (1)

typedef struct __super_t {
    int inode_bitmap_addr;
    int inode_bitmap_len;
    int data_bitmap_addr;
    int data_bitmap_len;
    int inode_region_addr;
    int inode_region_len;
    int data_region_addr;
    int data_region_len;
    int num_inodes;
    int num_data;
} super_t;

typedef struct __inode_t {
    int size;
    int type;
    int direct[DIRECT_PTRS];
    int indirect;
} inode_t;

typedef struct __dir_ent_t {
    int inum;
    char name[28];
} dir_ent_t;

#endif