// include/mfs.h
#ifndef MFS_H
#define MFS_H

#define MFS_DIRECTORY (0)
#define MFS_REGULAR_FILE (1)

#define MAX_FILENAME (28)
#define DIRECT_PTRS (30)

#define MFS_INIT 0
#define MFS_LOOKUP 1
#define MFS_STAT 2
#define MFS_WRITE 3
#define MFS_READ 4
#define MFS_CREAT 5
#define MFS_UNLINK 6

typedef struct __MFS_Stat_t {
    int type;   // MFS_DIRECTORY or MFS_REGULAR
    int size;   // bytes
} MFS_Stat_t;

int MFS_Init(const char *hostname, int port);
int MFS_Lookup(int pinum, const char *name);
int MFS_Stat(int inum, MFS_Stat_t *m);
int MFS_Write(int inum, char *buffer, int offset, int nbytes);
int MFS_Read(int inum, char *buffer, int offset, int nbytes);
int MFS_Creat(int pinum, int type, const char *name);
int MFS_Unlink(int pinum, const char *name);

#endif // MFS_H