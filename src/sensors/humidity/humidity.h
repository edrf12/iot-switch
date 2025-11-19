#ifndef HUMIDITY_H_DEF
#define HUMIDITY_H_DEF

#include <ArduinoHA.h>
#include <DHT_U.h>

class Humidity {
   private:
    HASensorNumber entity;
    DHT* dht;

    unsigned long last_humidity_publish = 0;

   public:
    Humidity(const char* id, const char* name, DHT* dht);

    void loop();
};

#endif  // HUMIDITY_H_DEF