#include "button.h"

Button::Button(const char* name, uint8_t pin)
    : entity(HADeviceTrigger::TriggerType::ButtonShortPressType,
             HADeviceTrigger::TriggerSubtype::Button1Subtype),
      pin(pin),
      name(name) {}

void Button::setup() {
    pinMode(pin, INPUT);
    entity.setName(name);
}

void Button::loop() {
    if (millis() - this->last_button_check > 12500 && digitalRead(pin)) {
        this->entity.trigger();
        this->last_button_check += 12500;
    }
}