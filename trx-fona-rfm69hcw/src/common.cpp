#ifndef common_cpp
#define common_cpp

#include "common.h"

void printString(uint8_t * buf, const uint8_t len) {
    for (uint8_t i = 0; i < len; i++)
        SerialMon.print((char)buf[i]);

    SerialMon.println("");
}

#endif
