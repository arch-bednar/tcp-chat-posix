//
// Created by Admin on 26/04/2025.
//
#include "client.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int createSocket(int *sockfd, struct sockaddr_in *addr, int port) {
    addr -> sin_addr.s_addr = inet_addr("127.0.0.1");
    addr -> sin_family = AF_INET;
    addr -> sin_port = port;

    int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socketfd == -1) {
        perror("Error during creating socket");
        return 1;
    }
    *sockfd = socketfd;
    return 0;
}

void start(int port) {
    int sockfd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);

    if (createSocket(&sockfd, &addr, port) == 0)
        return;

    if (connect(sockfd, (struct sockaddr*) &addr, addr_len) == -1) {
        perror("Connection error");
        return;
    }

    char data[1000];
    if (recv(sockfd, data, sizeof(data), 0)) {
        perror("Listening error");
        return;
    }else {
        printf("%s\n", data);
    }
}

int main() {
    start(5000);
}