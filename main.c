#include "server.h"
#include "can.h"

void startECU(char* name);
void handleRequest(int canfd, char* buf);

int main(void) {
    char buf[MAXLINE];
    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    char client_hostname[MAXLINE], client_port[MAXLINE];
    int canfd, listenfd, connfd;

    // ECU 작동 시작
    startECU("lockDoorECU");
    startECU("startCarECU");
    sleep(5);

    // can통신을 위한 file descriptor 만들기
    canfd = getCANfd("vcanMainECU");

    // socket으로 요청 받기
    listenfd = open_listenfd();
    while (1) {
        // 자바 서버와 socket 연결
        connfd = Accept(listenfd, (struct sockaddr*) &clientaddr, &clientlen);
        Getnameinfo((struct sockaddr*) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        
        Read(connfd, buf, MAXLINE); // 자바 서버로부터 요청을 buf로 읽기

        handleRequest(canfd, buf); // 요청에 따라 적절히 CAN통신 로직 수행

        close(connfd);
    }
    close(listenfd);
    return 0;
}

// 요청에 따라 적절히 CAN통신 로직 수행
void handleRequest(int canfd, char* buf) {
    struct can_frame frame;

    if (!strcmp("/lock-door", buf)) {
        frame.can_id = LOCK_DOOR_ID;
        frame.can_dlc = 8;
        memset(frame.data, 0x00, sizeof(frame.data));
    } else if (!strcmp("/unlock-door", buf)) {
        frame.can_id = UNLOCK_DOOR_ID;
        frame.can_dlc = 8;
        memset(frame.data, 0x00, sizeof(frame.data));
    } else if (!strcmp("/start-car", buf)) {
        frame.can_id = START_CAR_ID;
        frame.can_dlc = 8;
        memset(frame.data, 0x00, sizeof(frame.data));
    } else if (!strcmp("/stop-car", buf)) {
        frame.can_id = STOP_CAR_ID;
        frame.can_dlc = 8;
        memset(frame.data, 0x00, sizeof(frame.data));
    } else {
        fprintf(stderr, "not match request error\n");
        return;
    }

    Write(canfd, &frame, sizeof(frame));
    Read(canfd, &frame, sizeof(frame));
}

// 가상 ECU를 프로세스로 만들기
void startECU(char* name) {
    pid_t pid;
    char* argv[] = { NULL };
    char* envp[] = { NULL };
    char executeFile[MAXLINE] = "./";
    strcat(executeFile, name); // "./" + name

    if ((pid = fork()) < 0) {
        perror("fork error");
        exit(1);
    }
    if (pid == 0) { // child
        if (execve(executeFile, argv, envp) < 0) {
            perror("execve error");
            exit(1);
        }
    }
}