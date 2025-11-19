#ifndef BUTTON_H_DEF
#define BUTTON_H_DEF

#ifndef ARDUINOHA_USE_STD_FUNCTION
#define ARDUINOHA_USE_STD_FUNCTION
#endif  // ARDUINOHA_USE_STD_FUNCTION

#include <ArduinoHA.h>

class Button {
   private:
    HADeviceTrigger entity;

    uint8_t pin;

    unsigned long last_button_check = 0;

    const char* name;

   public:
    Button(const char* name, uint8_t pin);

    void setup();
    void loop();
};

#endif