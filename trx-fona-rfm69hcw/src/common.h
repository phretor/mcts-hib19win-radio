#ifndef common_h
#define common_h

#include <SPI.h>

/**
 * Contest in a nutshell:
 *
 * - the players are given a static signal capture containing a secret
 *   frequency
 * - this node transmits and receives at that frequency
 * - this node transmits a packet containing FLAG1 every TX_DELAY + RX_TIMEOUT
 *   seconds
 * - the players decode that FLAG1 and validate it via the webapp
 * - the players are told by the webapp that there is a printed label (FLAG2)
 *   on the
 *   node's enclosure, and that they have to locate the physical node
 * - the players find the node and the label and validate it via the webapp
 * - the webapp informs the players that the node is also receiving, and that
 *   they have to send them a specific payload (PAYLOAD) to unlock the last flag (FLAG3)
 * - once this node receives the right PAYLOAD, it will transmit TX_REPEAT
 *   times the message containing FLAG3
 */

#include "credentials.h"

//#define USE_MODEM     // remove comment to use a modem
#define SOFTWARE_SERIAL_MODEM_TX 8
#define SOFTWARE_SERIAL_MODEM_RX 9

#define RADIO_DEBUG

#define SYNCWORD "\x2d\xd4"
#define PAYLOAD "Ich4ng3d"
#define TX_DELAY 2000
#define TX_REPEAT 20
#define TX_POWER 20
#define RX_TIMEOUT 10000

// This is the ID transmitted periodically
#define FLAG1 "CH4nG3Me"

// This is printed on the node's external case label and must be visible
#define FLAG2 "Label3dG00dneSs"

#define FLAG3 "Unl3cked01"

// this is broadcasted all the time, the ID is meant to be submitted to the
// oracle web app
#define HELLO CONTEST_NAME "_Hello_[ID:" FLAG1 "]EOP"

// this is returned by the oracle web app once the right ID is submitted
#define SECRET_INNER "[ID:" PAYLOAD "]"
#define SECRET CONTEST_NAME "_Hello_" SECRET_INNER "EOP"

// this is the final secret, transmitted 16 times once the SECRET is recevied
// as payload by this node
#define FINAL CONTEST_NAME "_Hello_[Secret:" FLAG3 "]EOP"

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

void printString(uint8_t * buf, const uint8_t len);

#endif
