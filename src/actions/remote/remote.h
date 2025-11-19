#ifndef REMOTE_H_DEF
#define REMOTE_H_DEF 1

#ifndef ARDUINOHA_USE_STD_FUNCTION
#define ARDUINOHA_USE_STD_FUNCTION
#endif  // ARDUINOHA_USE_STD_FUNCTION

#include <ArduinoHA.h>
#include <DHT_U.h>
#include <IRac.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

class AC {
   private:
    HAHVAC entity;

    IRac sender;

    DHT* dht;

    uint8_t temperature = 24;
    float room_temperature = 24;

    uint8_t power = 0;
    uint8_t mode = 0;
    uint8_t fan_speed = 0;

    unsigned long last_temp_publish = 0;

    stdAc::opmode_t convert_mode(HAHVAC::Mode mode);
    stdAc::fanspeed_t convert_fan(HAHVAC::FanMode mode);

    void onPowerCommand(bool state, HAHVAC* sender);
    void onModeCommand(HAHVAC::Mode mode, HAHVAC* sender);
    void onFanModeCommand(HAHVAC::FanMode mode, HAHVAC* sender);
    void onTargetTemperatureCommand(HANumeric target, HAHVAC* sender);

   public:
    AC(const char* id, const char* name, uint8_t ir_pin, DHT* dht);

    void setup();
    void loop();
};
#endif  // REMOTE_H_DEF