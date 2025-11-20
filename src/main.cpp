#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ArduinoHA.h>
#include <DHT_U.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <Wire.h>
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

// SoftwareSerial COMSerial(D2, D3);
// HardwareSerial SerialH(0);
// Seeed_HSP24 xiao_config(SerialH);

AC ac("acEduardo", "AC", IRTX_PIN, &dht);
Switch lamp("luzPrincipal", "Luz", RELAY_PIN);

Button button("Botão de Cena", TOUCH_PIN);
Luminosity luminosity("luminosidade", "Luminosidade", LIGHT_PIN);
Humidity humidity("umidade", "Umidade", &dht);

HALight onboardLed("onboardLed");

HABinarySensor presense("presense");
unsigned long last_presense_publish = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool display_setup = false;
unsigned long last_display_show = 0;

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

void show_display() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.cp437();

    display.setTextSize(1.5);
    display.setCursor(5, 10);
    display.printf("Temperatura / Alvo");

    display.setTextSize(2);
    display.setCursor(20, 35);
    display.printf("%.0f/%.0f %cC", dht.readTemperature(), ac.getTarget(), 167);
    display.display();
}

bool target_to_bool(Seeed_HSP24::TargetStatus status) {
    switch (status) {
        case Seeed_HSP24::TargetStatus::NoTarget:
            return false;
        case Seeed_HSP24::TargetStatus::MovingTarget:
            return true;
        case Seeed_HSP24::TargetStatus::StaticTarget:
            return true;
        case Seeed_HSP24::TargetStatus::BothTargets:
            return true;
        default:
            return false;
    }
}

void setup() {
#ifdef DISPLAY_ONLY
    goto display_begin;
#endif
#ifdef DEBUG
    Serial.begin(9600);
#endif
    // SerialH.begin(9600);

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

    // Initialize presence sensor
    // COMSerial.begin(9600);
    // xiao_config.disableEngineeringModel();
    presense.setName("Presença");
    presense.setDeviceClass("occupancy");

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

    // Initialize display
    if (display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        display_setup = true;
    }
}

void loop() {
    mqtt.loop();
    ac.loop();
    button.loop();
    humidity.loop();
    luminosity.loop();

    // if (millis() - last_presense_publish > 5500) {
    //     last_presense_publish += 5500;

    //     if (xiao_config.getStatus().targetStatus !=
    //         Seeed_HSP24::TargetStatus::ErrorFrame) {
    //         presense.setState(
    //             target_to_bool(xiao_config.getStatus().targetStatus));
    //     }
    // }

    if (millis() - last_display_show > 30000 && display_setup) {
        last_display_show += 30000;
        show_display();
    }
}