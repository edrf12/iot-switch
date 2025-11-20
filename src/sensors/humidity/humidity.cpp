#include "humidity.h"

#define READING_INTERVAL 30000

Humidity::Humidity(const char* id, const char* name, DHT* dht)
    : entity(id, HABaseDeviceType::PrecisionP0), dht(dht) {
    entity.setName(name);
    entity.setUnitOfMeasurement("%");
}

void Humidity::loop() {
    if (millis() - last_humidity_publish > READING_INTERVAL) {
        last_humidity_publish += READING_INTERVAL;

        entity.setValue(dht->readHumidity());
    }
}