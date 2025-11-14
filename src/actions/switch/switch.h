#ifndef SWITCH_H_DEF
#define SWITCH_H_DEF 1

#ifndef ARDUINOHA_USE_STD_FUNCTION
#define ARDUINOHA_USE_STD_FUNCTION
#endif  // ARDUINOHA_USE_STD_FUNCTION

#include <ArduinoHA.h>

class Switch {
   private:
    HALight entity;

    bool relayState = false;
    bool lampState = false;

    uint8_t relayPin;

   public:
    Switch(const char* id, const char* name, uint8_t relay_pin);

    void onStateCommand(bool state, HALight* sender);

    void setup();
    void loop();
};
#endif  // SWITCH_H_DEF