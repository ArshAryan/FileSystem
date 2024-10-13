#include <stdio.h>
#include <arpa/inet.h>
#include "udp.h"

#define SERVER_PORT 12345
#define BUFFER_SIZE 4096

int main() {
    int sd = UDP_Open(0);
    if (sd < 0) {
        fprintf(stderr, "Failed to open socket\n");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    char buffer[BUFFER_SIZE] = "Hello, Server!";
    int rc = UDP_Write(sd, &server_addr, buffer, strlen(buffer) + 1);
    if (rc < 0) {
        perror("UDP_Write");
    }

    struct sockaddr_in reply_addr;
    rc = UDP_Read(sd, &reply_addr, buffer, BUFFER_SIZE);
    if (rc > 0) {
        printf("Received message: %s\n", buffer);
    } else {
        perror("UDP_Read");
    }

    UDP_Close(sd);
    return 0;
}
