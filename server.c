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
#include <stdlib.h>

struct t_user {
    char name[100]; //name
    char nickname[100]; //nickname
    int sockfd = 0; //socket file descriptor
    struct sockaddr addr; //address sockets
    pthread_t thread; //thread id
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
    t_user * client = (t_user*) user;

    while(flag) {
        if (client->name[0] == '\0' ) {
            char name[100], nickname[100];
            ssize_t bytes = 0;
            char buffer[100];
            char message[] = "Enter your name: ";

            send(client->sockfd, message, sizeof(message), 0);
            bytes = recv(client->sockfd, buffer, sizeof(buffer), 0);
            if (bytes == -1) {
                perror("Socke")
            }

        }
    }
    return NULL;
}

void clearHostsTable( t_user hosts[], struct t_user * empty_usr) {
    for (int i=0; i<100; i++) {
        hosts[i] = *empty_usr;
    }
}

void closeSockets() {
    for(int host=0; host<sizeof(hosts); host++) {
        if (hosts[host].sockfd > 0)
            close(hosts[host].sockfd);
    }
}

t_user * createEmptyUser() {
    t_user * user = malloc(sizeof(t_user));
    user -> thread=0;
    user -> name[0]='\0';
    user -> nickname[0]='\0';
    return user;
}

void start() {
    struct sockaddr_in sock;
    int sockfd;

    if (createSocket(&sockfd, &sock) == 1) {
        perror("Error during createSocket()");
        return;
    }
    printf("Socket created\n");

    if (bindSocket(sockfd, &sock) == 1) {
        perror("bindingSocket error: No port bound to socket");
        return;
    }
    printf("Socket binded\n");

    if (listen(sockfd, 100) == -1) {
        perror("Listen error");
        return;
    }

    printf("Listening\n");

    bool flag = true;
    struct sockaddr addr;
    socklen_t size = sizeof(struct sockaddr);

    t_user * empty_usr = createEmptyUser();

    clearHostsTable(hosts, empty_usr);
    printf("host table cleared\n");

    int clifd;

    // clifd = accept(sockfd, &addr, &size);

    // printf("Socket %d accepted\n", clifd);
    // char buffer[] = "Siema";
    // send(clifd, buffer, sizeof(buffer), 0);
    // printf("message sent\n");
    flag = true;
    while(flag) {
        clifd = accept(sockfd, &addr, &size);
        if (clifd == -1) {
            perror("Error during accepting connection");
            close(clifd);
        }else {
            for (int i=0; i<100; i++) {
                if (hosts[i].sockfd == 0) { //if address of host[i] id equal to empty user
                    //creating user and user's thread
                    t_user user;
                    user.addr = addr;
                    user.sockfd = clifd;
                    hosts[i] = user;
                    int result = pthread_create(&user.thread, NULL, handleUser, &hosts[i]);
                    if (result == -1) {
                        perror("Could not create thread");
                        close(clifd);
                        i=100;
                    }
                }
            }

            char buffer[] = "Siema";
            recv(clifd, buffer, sizeof(buffer), 0);
            flag = false;
        }
    }
    printf("Closing sockets...");
    closeSockets();
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