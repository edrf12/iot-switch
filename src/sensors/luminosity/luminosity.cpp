#include "luminosity.h"

#define READING_INTERVAL 30000

Luminosity::Luminosity(const char* id, const char* name, uint8_t sensor_pin)
    : entity(id, HABaseDeviceType::PrecisionP0), sensorPin(sensor_pin) {
    entity.setName(name);
    entity.setUnitOfMeasurement("lx");
    pinMode(sensorPin, INPUT);
}

void Luminosity::loop() {
    if (millis() - last_luminosity_publish > READING_INTERVAL) {
        last_luminosity_publish += READING_INTERVAL;

        analogReadResolution(10);
        float volts = analogRead(sensorPin) * 5 / 1024.0;
        float amps = volts / 10000.0;
        float microamps = amps * 1000000;
        float lux = microamps * 2.0;

        entity.setValue(lux);

#ifdef DEBUG
        Serial.print("Raw Luminosity: ");
        Serial.print(rawValue);
        Serial.print(" -> Mapped: ");
        Serial.println(mappedValue);
#endif  // DEBUG
    }
}