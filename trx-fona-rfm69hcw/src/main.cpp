#include "common.h"
#include "radio.h"

#define BUILTIN_LED 13

#ifdef USE_MODEM

#define TINY_GSM_MODEM_SIM800
#define MQTT_CLIENT_NAME CONTEST_NAME

#include <SoftwareSerial.h>
SoftwareSerial SerialAT(SOFTWARE_SERIAL_MODEM_TX, SOFTWARE_SERIAL_MODEM_RX);

// See all AT commands, if wanted
//#define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
//#define TINY_GSM_DEBUG SerialMon

// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

// Add a reception delay - may be needed for a fast processor at a slow baud
// rate
//#define TINY_GSM_YIELD() { delay(2); }

// Define how you're planning to connect to the internet
#define TINY_GSM_USE_GPRS true

// Your GPRS credentials, if any
const char apn[] = GPRS_APN;
const char gprsUser[] = GPRS_USER;
const char gprsPass[] = GPRS_PASS;

// MQTT details
const char *broker = MQTT_HOST;

const char *topicOutStatus = MQTT_CLIENT_NAME "/out/status";
const char *topicInStatus = MQTT_CLIENT_NAME "/in/status";
const char *topicOutRadio = MQTT_CLIENT_NAME "/out/radio";
const char *topicInRadio = MQTT_CLIENT_NAME "/in/radio";

#include <PubSubClient.h>
#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);
PubSubClient mqtt(client);

uint8_t nodeStatus = 1;

long lastReconnectAttempt = 0;

void mqttCallback(char *topic, byte *payload, unsigned int len) {
  SerialMon.print("Message arrived [");
  SerialMon.print(topic);
  SerialMon.print("]: ");
  SerialMon.write(payload, len);
  SerialMon.println();

  if (String(topic) == topicInStatus)
    mqtt.publish(topicOutStatus, String(nodeStatus).c_str());

  if (String(topic) == topicInRadio)
    mqtt.publish(topicOutRadio, String(get_radio_status()).c_str());
}

boolean mqttConnect() {
  SerialMon.print("Connecting to ");
  SerialMon.print(broker);

  boolean status =
      mqtt.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASS, topicOutStatus, 1, 1,
                   "0"); // last will testament

  if (status == false) {
    SerialMon.println(": fail");
    return false;
  }

  SerialMon.println(": success");
  mqtt.publish(topicOutStatus, String(nodeStatus).c_str());
  mqtt.publish(topicOutRadio, String(get_radio_status()).c_str());

  // Subscribe to these topics
  mqtt.subscribe(topicInStatus);
  mqtt.subscribe(topicInRadio);

  // Connect to broker
  return mqtt.connected();
}
#endif // USE_MODEM

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  SerialMon.begin(115200);
  delay(10);

  SerialMon.println("Wait...");

#ifdef USE_MODEM
  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);
#endif

  // Setup radio
  radio_setup();

#ifdef USE_MODEM
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // modem.init();

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);

#if TINY_GSM_USE_GPRS
  // Unlock your SIM card with a PIN if needed
  if (GSM_PIN && modem.getSimStatus() != 3) {
    modem.simUnlock(GSM_PIN);
  }
#endif

  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(": success");

  if (modem.isNetworkConnected())
    SerialMon.println("Network connected");

#if TINY_GSM_USE_GPRS
  // GPRS connection parameters are usually set after network registration
  SerialMon.print(F("Connecting to "));
  SerialMon.print(GPRS_APN);
  if (!modem.gprsConnect(GPRS_APN, GPRS_USER, GPRS_PASS)) {
    SerialMon.println(": fail");
    delay(5000);
    return;
  }
  SerialMon.println(": success");

  if (modem.isGprsConnected())
    SerialMon.println("GPRS connected");
#endif

  // MQTT Broker setup
  mqtt.setServer(broker, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
#endif
}

void loop() {
#ifdef USE_MODEM
  if (!mqtt.connected()) {
    unsigned long t = millis();
    if (t - lastReconnectAttempt > 10000L) {
      lastReconnectAttempt = t;
      if (mqttConnect()) {
        lastReconnectAttempt = 0;
      }
    }
    delay(100);
    return;
  }

  mqtt.loop();
#endif

  radio_loop();
}
