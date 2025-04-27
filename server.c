//
// Created by Admin on 26/04/2025.
//

#include "server.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct t_user {
    char name[100];
    char nickname[100];
    int sockfd;
};

t_user hosts[100]; //table of file descriptors of hosts

int createSocket(int *sockfd, struct sockaddr_in *addr) {
    // sock -> sa_family = AF_INET;
    // sock -> sa_len = sizeof(struct sockaddr);
    addr -> sin_family = AF_INET;
    //sock -> sin_addr.s_addr = htonl((127 << 24) | (0 << 16) | (0 << 8 ) | 1);
    //inet_pton(AF_INET, "192.168.0.1", &(addr.sin_addr));
    addr -> sin_addr.s_addr = inet_addr("127.0.0.1");
    //sock -> sin_port = htons(65000);
    addr -> sin_len = sizeof(addr);

    *sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (*sockfd == -1) {
        return 1;
    }
    return 0;
}

int bindSocket(int sockfd, struct sockaddr_in *addr){
//function to bind socket to free port
    for (int port = 50000; port < 255*255; port++) {
        addr -> sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr*)addr, sizeof(struct sockaddr_in)) == -1) {
            printf("bindingPort: Binding to port %d failed\n", port);
        }else {
            printf("bindingPort: Binding to port %d - success\n", port);
            return 0;
        }
    }

    return 1;
}

void closeConnection(int sockfd) {
    if(close(sockfd))
        perror("Closing error");
    printf("Closing...");
}

void beginListening() {

}

void start() {
    struct sockaddr_in sock;
    int sockfd;

    if (createSocket(&sockfd, &sock) == 1) {
        perror("Error during createSocket()");
        return;
    }

    if (bindSocket(sockfd, &sock) == 1) {
        perror("bindingSocket error: No port bound to socket");
        return;
    }





}