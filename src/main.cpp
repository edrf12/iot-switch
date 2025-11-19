#include <ArduinoHA.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <mmwave_for_xiao.h>

#include "actions/remote/remote.h"
#include "actions/switch/switch.h"
#include "pins.h"
#include "secrets.h"
#include "sensors/button/button.h"
#include "sensors/humidity/humidity.h"
#include "sensors/luminosity/luminosity.h"

HADevice device;
WiFiClient client;
HAMqtt mqtt(client, device);

DHT dht(DHT_PIN, DHT_TYPE);

SoftwareSerial COMSerial(D2, D3);
Seeed_HSP24 xiao_config(COMSerial);

AC ac("acEduardo", "AC", IRTX_PIN, &dht);
Switch lamp("luzPrincipal", "Luz", RELAY_PIN);

Button button("Botão de Cena", TOUCH_PIN);
Luminosity luminosity("luminosidade", "Luminosidade", LIGHT_PIN);
Humidity humidity("umidade", "Umidade", &dht);

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
    device.setName("Alpha Switch");
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

    // Initialize dht before all entities
    dht.begin();

    // Initialize proximity sensor
    xiao_config.disableEngineeringModel();

    // Begin MQTT
    mqtt.setKeepAlive(90);
    mqtt.begin(MQTT_BROKER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);

    // Setup onboardLed
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    onboardLed.setName("Onboard LED");
    onboardLed.onStateCommand(onStateCommand);

    // Initialize entities
    ac.setup();
    lamp.setup();
    button.setup();
}

void loop() {
    mqtt.loop();
    ac.loop();
    button.loop();
    luminosity.loop();
    humidity.loop();
}