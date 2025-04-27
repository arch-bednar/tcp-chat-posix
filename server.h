//
// Created by Admin on 26/04/2025.
//

#ifndef SERVER_H
#define SERVER_H
typedef struct t_user t_user;
//table of file descriptors of hosts


int createSocket(struct sockaddr_in sock);
int bindSocket(int sockfd, const struct sockaddr *addr);
void closeConnection();
void start();
void beginListening();
#endif //SERVER_H
