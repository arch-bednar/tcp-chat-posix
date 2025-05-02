//
// Created by Admin on 26/04/2025.
//
#include "client.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int createSocket(int *sockfd, struct sockaddr_in *addr, int port) {
    addr -> sin_addr.s_addr = inet_addr("127.0.0.1");
    addr -> sin_family = AF_INET;
    addr -> sin_port = htons(port);
#ifdef __APPLE__
    addr -> sin_len = sizeof(struct sockaddr_in);
#endif

    int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socketfd == -1) {
        perror("Error during creating socket");
        return 1;
    }
    *sockfd = socketfd;
    return 0;
}

void start(char port[]) {
    int sockfd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    if (createSocket(&sockfd, &addr, atoi(port)) == 1)
        return;

    printf("%d\n", sockfd);

    if (connect(sockfd, (struct sockaddr*) &addr, addr_len) == -1) {
        perror("Connection error");
        return;
    }

    char data[1000];
    ssize_t bytes = recv(sockfd, data, sizeof(data), 0);
    if (bytes == -1) {
        printf("Listening error\n");
        return;
    }
    else if ( bytes == 0) {
        perror("Socket error");
        return;
    }else {
        printf("data: %s\n", data);
    }

    close(sockfd);
}

int main(int argc, char *argv[]) {
    start(argv[1]);
}