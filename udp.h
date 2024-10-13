#ifndef UDP_H
#define UDP_H

int UDP_Open(int port);
int UDP_Close(int fd);
int UDP_Read(int fd, struct sockaddr_in *addr, char *buffer, int n);
int UDP_Write(int fd, struct sockaddr_in *addr, char *buffer, int n);

#endif