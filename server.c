#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "udp.h"
#include "mfs.h"

#define BUFFER_SIZE 4096

typedef struct {
    int cmd;
    int pinum;
    int type;
    int inum;
    int offset;
    int nbytes;
    char name[28];
    char buffer[BUFFER_SIZE];
} message_t;

int sd;

void handle_request(message_t *msg, struct sockaddr_in *client) {
    message_t reply;
    memset(&reply, 0, sizeof(reply));

    switch (msg->cmd) {
    case MFS_INIT:
        reply.cmd = MFS_INIT;
        break;
    case MFS_LOOKUP:
        reply.inum = MFS_Lookup(msg->pinum, msg->name);
        reply.cmd = MFS_LOOKUP;
        break;
    case MFS_STAT:
        reply.cmd = MFS_STAT;
        if (MFS_Stat(msg->inum, (MFS_Stat_t *)reply.buffer) < 0)
            reply.inum = -1;
        break;
    case MFS_WRITE:
        reply.cmd = MFS_WRITE;
        reply.inum = MFS_Write(msg->inum, msg->buffer, msg->offset, msg->nbytes);
        break;
    case MFS_READ:
        reply.cmd = MFS_READ;
        reply.inum = MFS_Read(msg->inum, reply.buffer, msg->offset, msg->nbytes);
        break;
    case MFS_CREAT:
        reply.cmd = MFS_CREAT;
        reply.inum = MFS_Creat(msg->pinum, msg->type, msg->name);
        break;
    case MFS_UNLINK:
        reply.cmd = MFS_UNLINK;
        reply.inum = MFS_Unlink(msg->pinum, msg->name);
        break;
    default:
        fprintf(stderr, "Unknown command: %d\n", msg->cmd);
        return;
    }

    UDP_Write(sd, client, (char *)&reply, sizeof(reply));
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <file-system-image>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    fs_init(argv[2]);

    sd = UDP_Open(port);
    if (sd < 0) {
        perror("UDP_Open");
        exit(1);
    }

    while (1) {
        struct sockaddr_in client;
        message_t msg;
        int rc = UDP_Read(sd, &client, (char *)&msg, sizeof(msg));
        if (rc > 0) {
            handle_request(&msg, &client);
        }
    }

    fs_shutdown();
    return 0;
}