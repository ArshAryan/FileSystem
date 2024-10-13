// src/mfs.c
#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "mfs.h"
#include "fs.h"
#include "ufs.h"
#include "bitmap.h"

static filesystem_t *fs;

int MFS_Init(const char *hostname, int port) {
    // Connect to the server (placeholder code)
    // Implement UDP initialization and connection
    fs = fs_load("fs_image"); // Replace with the actual image file
    return (fs != NULL) ? 0 : -1;
}

int MFS_Lookup(int pinum, const char *name) {
    inode_t *parent_inode = get_inode(fs, pinum);
    if (!parent_inode || parent_inode->type != MFS_DIRECTORY) return -1;

    for (int i = 0; i < DIRECT_PTRS; i++) {
        int block = parent_inode->direct[i];
        if (block == -1) continue;

        dir_ent_t *dir = (dir_ent_t *)(get_block(fs, block));
        for (int j = 0; j < UFS_BLOCK_SIZE / sizeof(dir_ent_t); j++) {
            if (strcmp(dir[j].name, name) == 0) {
                return dir[j].inum;
            }
        }
    }

    return -1;
}

int MFS_Stat(int inum, MFS_Stat_t *m) {
    inode_t *inode = get_inode(fs, inum);
    if (!inode) return -1;

    m->type = inode->type;
    m->size = inode->size;
    return 0;
}

int MFS_Write(int inum, char *buffer, int offset, int nbytes) {
    inode_t *inode = get_inode(fs, inum);
    if (!inode || inode->type != MFS_REGULAR_FILE) return -1;

    int block = inode->direct[0];
    if (block == -1) return -1;

    memcpy(get_block(fs, block) + offset, buffer, nbytes);
    inode->size = offset + nbytes;
    return 0;
}

int MFS_Read(int inum, char *buffer, int offset, int nbytes) {
    inode_t *inode = get_inode(fs, inum);
    if (!inode || inode->type != MFS_REGULAR_FILE) return -1;

    int block = inode->direct[0];
    if (block == -1) return -1;

    memcpy(buffer, get_block(fs, block) + offset, nbytes);
    return 0;
}

int MFS_Creat(int pinum, int type, const char *name) {
    return fs_creat(fs, pinum, type, name);
}

int MFS_Unlink(int pinum, const char *name) {
    inode_t *parent_inode = get_inode(fs, pinum);
    if (!parent_inode || parent_inode->type != MFS_DIRECTORY) return -1;

    for (int i = 0; i < DIRECT_PTRS; i++) {
        int block = parent_inode->direct[i];
        if (block == -1) continue;

        dir_ent_t *dir = (dir_ent_t *)(get_block(fs, block));
        for (int j = 0; j < UFS_BLOCK_SIZE / sizeof(dir_ent_t); j++) {
            if (strcmp(dir[j].name, name) == 0) {
                int inum = dir[j].inum;
                inode_t *inode = get_inode(fs, inum);
                if (inode->type == MFS_DIRECTORY) {
                    // Check if directory is empty
                    for (int k = 0; k < DIRECT_PTRS; k++) {
                        int dblock = inode->direct[k];
                        if (dblock == -1) continue;

                        dir_ent_t *subdir = (dir_ent_t *)(get_block(fs, dblock));
                        for (int l = 0; l < UFS_BLOCK_SIZE / sizeof(dir_ent_t); l++) {
                            if (subdir[l].inum != -1) return -1;
                        }
                    }
                }
                bitmap_clear(fs->inode_bitmap, inum);
                memset(&dir[j], 0, sizeof(dir_ent_t));
                return 0;
            }
        }
    }
    return -1;
}

inode_t *get_inode(filesystem_t *fs, int inum) {
    if (inum < 0 || inum >= fs->super.num_inodes) return NULL;
    return &fs->inodes[inum];
}

void *get_block(filesystem_t *fs, int block) {
    if (block < 0 || block >= fs->super.num_data) return NULL;
    return fs->data_blocks + block * UFS_BLOCK_SIZE;
}