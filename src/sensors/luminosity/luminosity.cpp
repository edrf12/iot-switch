#include "luminosity.h"

#define READING_INTERVAL 60000

Luminosity::Luminosity(const char* id, const char* name, uint8_t sensor_pin)
    : entity(id, HABaseDeviceType::PrecisionP0), sensorPin(sensor_pin) {
    entity.setName(name);
    entity.setUnitOfMeasurement("%");
}

void Luminosity::loop() {
    if (millis() - last_luminosity_publish >= READING_INTERVAL) {
        last_luminosity_publish += READING_INTERVAL;

        int rawValue = analogRead(sensorPin);

        long mappedValue = map(rawValue, 0, RAW_MAX, 0, MAPPED_MAX);

        mappedValue = constrain(mappedValue, 0, MAPPED_MAX);

        entity.setCurrentValue(mappedValue);

#ifdef DEBUG
        Serial.print("Raw Luminosity: ");
        Serial.print(rawValue);
        Serial.print(" -> Mapped: ");
        Serial.println(mappedValue);
#endif  // DEBUG
    }
}