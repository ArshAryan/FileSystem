// include/fs.h
#ifndef FS_H
#define FS_H

#include "ufs.h"
#include "bitmap.h"
#include "mfs.h"

typedef struct {
    super_t super;
    bitmap_t *inode_bitmap;
    bitmap_t *data_bitmap;
    inode_t *inodes;
    char *data_blocks;
} filesystem_t;

filesystem_t *fs_load(const char *image_file);
void fs_free(filesystem_t *fs);
int fs_lookup(filesystem_t *fs, int pinum, const char *name);
int fs_stat(filesystem_t *fs, int inum, MFS_Stat_t *m);
int fs_write(filesystem_t *fs, int inum, char *buffer, int offset, int nbytes);
int fs_read(filesystem_t *fs, int inum, char *buffer, int offset, int nbytes);
int fs_creat(filesystem_t *fs, int pinum, int type, const char *name);
int fs_unlink(filesystem_t *fs, int pinum, const char *name);

inode_t *get_inode(int inum);
void *get_block(int block);

#endif