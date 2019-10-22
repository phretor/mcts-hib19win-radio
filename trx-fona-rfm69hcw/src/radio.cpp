#ifndef radio_cpp
#define radio_cpp

#include "radio.h"

#include <string.h>

uint8_t radioStatus = 0;
RH_RF69 rf69(RFM69_SS, RFM69_IRQ);

uint8_t get_radio_status() { return radioStatus; }

void radio_setup() {
  if (!rf69.init()) {
    SerialMon.println("RFM69 init failed");
    return;
  }

  rf69.setModemConfig(RH_RF69::OOK_Rb1Bw1);
  rf69.spiWrite(
      RH_RF69_REG_37_PACKETCONFIG1,
      0x80); // variable len, no CRC, no address filtering, no whitening

#ifdef RADIO_DEBUG
  rf69.printRegisters();
#endif

#ifdef SYNCWORD
  rf69.setSyncWords((uint8_t *)SYNCWORD, sizeof((uint8_t *)SYNCWORD));
#endif

#ifdef RFM69_FREQ
  if (!rf69.setFrequency(RFM69_FREQ)) {
    SerialMon.println("RFM69 setFrequency failed");
    return;
  }
#endif

  rf69.setTxPower(TX_POWER, true);

  SerialMon.println("RFM69 initialized!");

  radioStatus = 1;
}

void radio_loop() {
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  len = sizeof(HELLO);
  rf69.send(HELLO, len);
  rf69.waitPacketSent();

#ifdef RADIO_DEBUG
  SerialMon.print("TX (");
  SerialMon.print(len);
  SerialMon.print(") = ");
  printString(HELLO, len);
#endif

  // Now wait for a packet

  if (rf69.waitAvailableTimeout(RX_TIMEOUT)) {
    if (rf69.recv(buf, &len)) {
#ifdef RADIO_DEBUG
      SerialMon.print("RX = ");
      printString(buf, len);

      SerialMon.print("Searching for pattern: '");
      SerialMon.print(SECRET_INNER);
      SerialMon.println("'");
#endif

      // See if the secret is included (we're relaxed here)
      if (strstr(buf, SECRET_INNER) != NULL) {
        for (uint8_t i = 0; i < TX_REPEAT; i++) {
          len = sizeof(FINAL);
          rf69.send(FINAL, len);
          rf69.waitPacketSent();
          delay(TX_DELAY);

#ifdef RADIO_DEBUG
          SerialMon.print("TX (");
          SerialMon.print(len);
          SerialMon.print(") = ");
          printString(FINAL, len);
#endif
        }
      }
    } else
      SerialMon.println("RFM69 RX error");
  }

  delay(TX_DELAY);
}
#endif
