#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "udp.h"

int UDP_Open(int port) {
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sd);
        return -1;
    }

    return sd;
}

int UDP_Close(int fd) {
    return close(fd);
}

int UDP_Read(int fd, struct sockaddr_in *addr, char *buffer, int n) {
    socklen_t len = sizeof(*addr);
    return recvfrom(fd, buffer, n, 0, (struct sockaddr *)addr, &len);
}

int UDP_Write(int fd, struct sockaddr_in *addr, char *buffer, int n) {
    return sendto(fd, buffer, n, 0, (struct sockaddr *)addr, sizeof(*addr));
}