#include <ArduinoHA.h>

#ifdef BOARD_PORTENTA
#include "WiFiC3.h"
#else
#include "WiFi.h"
#endif

#include "actions/remote/remote.h"
#include "pins.h"
#include "secrets.h"

HADevice device;
WiFiClient client;
HAMqtt mqtt(client, device);

AC ac("acEduardo", "AC", IRTX_PIN, DHT_PIN, DHT_TYPE);
HALight onboardLed("onboardLed");

void onStateCommand(bool state, HALight* sender) {
#ifdef DEBUG
    Serial.print("State: ");
    Serial.println(state);
#endif

    if (state) {
        digitalWrite(LED_BUILTIN, LOW);
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
    }

    sender->setState(state);
}

void setup() {
#ifdef DEBUG
    Serial.begin(9600);
#endif

    byte mac[6];
    WiFi.macAddress(mac);

    // Setup device on HASS
    device.setName("ESP");
    device.setSoftwareVersion("1.0.0");
    device.setUniqueId(mac, sizeof(mac));
    device.enableSharedAvailability();
    device.enableLastWill();

    // Connect to WiFi
    while (WiFi.status() != WL_CONNECTED) {
#ifdef DEBUG
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(WIFI_SSID);
#endif

#ifndef WIFI_PASSWORD
        WiFi.begin(WIFI_SSID);
#else
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

        delay(10000);
    }

#ifdef DEBUG
    Serial.println("You're connected to the network");
    Serial.println(WiFi.localIP().toString());
    Serial.println(WiFi.gatewayIP().toString());
#endif

    // Begin MQTT
    mqtt.setKeepAlive(90);
    mqtt.begin(MQTT_BROKER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);

    // Setup onboardLed
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    onboardLed.setName("Onboard LED");
    onboardLed.onStateCommand(onStateCommand);

    ac.setup();
}

void loop() {
    mqtt.loop();
}