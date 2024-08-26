#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVER_PORT      10000
#define MAXLINE          1024

int open_listenfd(void);

void Getnameinfo(struct sockaddr* sockaddr, socklen_t socklen, char* host, 
                 size_t hostlen, char* port, size_t portlen, int flags);

int Socket(int domain, int type, int protocol);
void Setsockopt(int listenfd, int level, int optname, int* optval, int optlen);
void Bind(int listenfd, struct sockaddr* serveraddr, int serveraddrlen);
void Listen(int listenfd, int listenq);
int Accept(int listenfd, struct sockaddr *clientaddr, socklen_t* clientlen);

void Read(int fd, void* buf, int size);
void Write(int fd, void* buf, int size);

#endif