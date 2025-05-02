//
// Created by Admin on 26/04/2025.
//

#ifndef CLIENT_H
#include <netinet/in.h>
int createSocket(int *sockfd, struct sockaddr_in *addr, int port);
void start(char port[]);

#define CLIENT_H

#endif //CLIENT_H
