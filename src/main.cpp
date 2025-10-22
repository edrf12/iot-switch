#define TRANS_PIN 1

#include <ArduinoHA.h>
#include <WiFiC3.h>

#include "secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

HADevice device;
WiFiClient client;
HAMqtt mqtt(client, device);

HALight onboardLed("onboardLed");
HASwitch transSwitch("transSwitch");

void onStateCommand(bool state, HALight* sender) {
    Serial.print("State: ");
    Serial.println(state);

    if (state) {
        digitalWrite(LED_BUILTIN, LOW);
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
    }

    sender->setState(state);  // report state back to the Home Assistant
}

void onTransCommand(bool state, HASwitch* sender) {
    digitalWrite(TRANS_PIN, state);
    sender->setState(state);
}

void setup() {
    byte mac[WL_MAC_ADDR_LENGTH];
    WiFi.macAddress(mac);

    // Setup device on HASS
    device.setName("Arduino");
    device.setSoftwareVersion("1.0.0");
    device.setUniqueId(mac, sizeof(mac));

    // Begin Serial for debugging
    Serial.begin(9600);
    while (!Serial) {
        ;  // wait for serial port to connect. Needed for native USB port only
    }

    // Connect to WiFi
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }

    Serial.println("You're connected to the network");

    // Begin MQTT
    mqtt.begin(MQTT_BROKER, MQTT_USER, MQTT_PASSWORD);

    // Setup onboardLed
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    onboardLed.setName("Onboard LED");
    onboardLed.onStateCommand(onStateCommand);

    // Setup trans
    pinMode(TRANS_PIN, OUTPUT);
    transSwitch.setName("Transistor");
    transSwitch.onCommand(onTransCommand);

    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
}

void loop() { mqtt.loop(); }