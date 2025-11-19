#ifndef LUMINOSITY_H_DEF
#define LUMINOSITY_H_DEF

#include <ArduinoHA.h>

class Luminosity {
   private:
    HASensorNumber entity;
    uint8_t sensorPin;
    const long RAW_MAX = 255;
    const long MAPPED_MAX = 100;

    unsigned long last_luminosity_publish = 0;

   public:
    Luminosity(const char* id, const char* name, uint8_t sensor_pin);

    void loop();
};

#endif  // LUMINOSITY_H_DEF