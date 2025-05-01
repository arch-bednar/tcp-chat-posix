//
// Created by Admin on 26/04/2025.
//

#include "server.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>

struct t_user {
    char name[100];
    char nickname[100];
    int sockfd;
    struct sockaddr addr;
    pthread_t thread;
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
    #ifdef __APPLE__
        addr -> sin_len = sizeof(addr);
    #endif

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

void *handleUser(void * user) {
    bool flag = true;

    while(flag) {
        
    }

    return 0;
}

void clearHostsTable( t_user * hosts[], struct t_user * empty_usr) {
    for (int i=0; i<100; i++) {
        hosts[i] = empty_usr;
    }
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

    if (listen(sockfd, 100) == -1) {
        perror("Listen error");
        return;
    }

    bool flag = true;
    struct sockaddr addr;
    socklen_t size = sizeof(struct sockaddr);

    struct t_user empty_usr;
    clearHostsTable(hosts, &empty_usr);

    int clifd;
    while(flag) {
        clifd = accept(sockfd, &addr, &size);
        if (clifd == -1) {
            perror("Error during accepting connection");
        }else {
            // for (int i=0; i<100; i++) {
            //     if (&hosts[i] == &empty_usr) {
            //         t_user user;
            //         user.addr = addr;
            //         user.sockfd = clifd;
            //         int result = pthread_create(&user.thread, NULL, handleUser, &user);
            //         if (result == -1)
            //             perror("Could not create thread");
            //     }
            // }

            char buffer[1000];
            recv(clifd, buffer, sizeof(buffer), 0);
            flag = false;
        }
    }
    close(clifd);
    close(sockfd);
    //FINISHING ALL THREADS
    // for (int i=0; i < 100; i++) {
    //     if(&hosts[i] != &empty_usr)
    //         pthread_join(hosts[i].thread, NULL);
    // }
}

int main() {
    start();
}