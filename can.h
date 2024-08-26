#ifndef __CAN_H__
#define __CAN_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#define LOCK_DOOR_ID    1
#define UNLOCK_DOOR_ID  2
#define START_CAR_ID    3
#define STOP_CAR_ID    4

#define POSITIVE_RESPONSE 100

int getCANfd(char* name);

#endif