//
// Created by Admin on 26/04/2025.
//

#ifndef SERVER_H
#define SERVER_H
#include <netinet/in.h>

typedef struct t_user t_user;
//table of file descriptors of hosts


int createSocket(int *sockfd, struct sockaddr_in *addr);
int bindSocket(int sockfd, struct sockaddr_in *addr);
void closeConnection(int sockfd);
void start();
void *handleUser(void * user);
void closeSockets();
#endif //SERVER_H
