#include "server.h"
#include "can.h"

int main(void) {
    int canfd;
    struct can_frame frame;
    bool isCarStarted = false;

    // CAN통신 연결
    canfd = getCANfd("vcanStartCarECU");

    // ECU 작동 상태
    while (1) {
        Read(canfd, &frame, sizeof(frame));
 
        if (frame.can_id == START_CAR_ID) {
            isCarStarted = true;

            frame.can_id = POSITIVE_RESPONSE;
            frame.can_dlc = 8;
            memset(frame.data, 0x02, sizeof(frame.data));
            Write(canfd, &frame, sizeof(frame));
        } else if (frame.can_id == STOP_CAR_ID) {
            isCarStarted = false;

            frame.can_id = POSITIVE_RESPONSE;
            frame.can_dlc = 8;
            memset(frame.data, 0x02, sizeof(frame.data));
            Write(canfd, &frame, sizeof(frame));
        }
    }
}