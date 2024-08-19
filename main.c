#include "server.h"

int main(void) {
    char buf[MAXLINE];
    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    char client_hostname[MAXLINE], client_port[MAXLINE];
    int n, connfd, listenfd = open_listenfd();

    while (1) {
        connfd = Accept(listenfd, (struct sockaddr*) &clientaddr, &clientlen);
        Getnameinfo((struct sockaddr*) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        
        close(connfd);
    }
    close(listenfd);
    return 0;
}