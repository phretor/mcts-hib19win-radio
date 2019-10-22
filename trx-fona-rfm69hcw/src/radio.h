#ifndef radio_h
#define radio_h

#include <RH_RF69.h>
#include <SPI.h>

#include "common.h"

#define RFM69_SS 10  // "B"
#define RFM69_RST 11 // "A"
#define RFM69_IRQ 2  // "SDA" (only SDA/SCL/RX/TX have IRQ on the 32u4)

#define RFM69_FREQ 444.0
#define TX_LIMIT 16

extern uint8_t radioStatus;

extern RH_RF69 rf69;

uint8_t get_radio_status();
void radio_setup();
void radio_loop();
#endif
