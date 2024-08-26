#include "server.h"
#include "can.h"

int main(void) {
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    int canfd;
    bool isDoorLocked = true;

    // CAN통신 연결
    canfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (canfd < 0) {
        perror("socket error");
        exit(1);
    }

    strcpy(ifr.ifr_name, "vcanLockDoorECU");
    if (ioctl(canfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl error");
        exit(1);
    }
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(canfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind error");
        exit(1);
    }


    // ECU 작동 상태
    while (1) {
        Read(canfd, &frame, sizeof(frame));
 
        if (frame.can_id == LOCK_DOOR_ID) {
            isDoorLocked = true;

            frame.can_id = POSITEIVE_RESPONSE;
            frame.can_dlc = 8;
            memset(frame.data, 0x02, sizeof(frame.data));
            Write(canfd, &frame, sizeof(frame));
        }
    }
}