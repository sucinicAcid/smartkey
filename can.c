#include "can.h"

int getCANfd(char* name) {
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    int s;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) {
        perror("socket error");
        exit(1);
    }

    strcpy(ifr.ifr_name, name);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl error");
        exit(1);
    }
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind error");
        exit(1);
    }

    return s;
}