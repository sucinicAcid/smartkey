#include "server.h"

int open_listenfd(void) {
	struct sockaddr_in serveraddr;
	char buf[MAXLINE];
	int listenfd, opt = 1;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(SERVER_PORT);
	Bind(listenfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
	
	Listen(listenfd, 1);

	return listenfd;
}

void Getnameinfo(struct sockaddr* sockaddr, socklen_t socklen, char* host, 
                 size_t hostlen, char* port, size_t portlen, int flags) {
	if (getnameinfo(sockaddr, socklen, host, hostlen, port, portlen, flags) != 0) {
		perror("getnameinfo error");
		exit(1);
	}
}

int Socket(int domain, int type, int protocol) {
    int n;
    if ((n = socket(domain, type, protocol)) < 0) {
		perror("socket error");
		exit(1);
	}
    return n;
}

void Setsockopt(int listenfd, int level, int optname, int* optval, int optlen) {
    if (setsockopt(listenfd, level, optname, optval, optlen) < 0) {
		perror("setsockopt error");
		exit(1);
	}
}

void Bind(int listenfd, struct sockaddr* serveraddr, int serveraddrlen) {
	if (bind(listenfd, serveraddr, serveraddrlen) < 0) {
		perror("bind error");
		exit(1);
	}
}

void Listen(int listenfd, int listenq) {
	if (listen(listenfd, listenq) < 0) {
		perror("listen error");
		exit(1);
	}
}

int Accept(int listenfd, struct sockaddr *clientaddr, socklen_t* clientlen) {
	int n;
	if ((n = accept(listenfd, clientaddr, clientlen)) < 0) {
		perror("accept error");
		exit(1);
	}
	return n;
}

void Read(int fd, void* buf, int size) {
    int nread;
    char *bufp = buf;

    while (size > 0) {
        if ((nread = read(fd, bufp, size)) < 0) {
            perror("read error");
			exit(1);
        } 
        else if (nread == 0)
            break;				// EOF
        size -= nread;
        bufp += nread;
    }
}

void Write(int fd, void* buf, int size) {
    int nwritten;
    char *bufp = buf;
	
    while (size > 0) {
		if ((nwritten = write(fd, bufp, size)) <= 0) {
			perror("write error");
			exit(1);
		}
		size -= nwritten;
		bufp += nwritten;
    }
}