#include "switch.h"

Switch::Switch(const char* id, const char* name, uint8_t relay_pin)
    : entity(id), relayPin(relay_pin) {
    entity.setName(name);
}

void Switch::onStateCommand(bool state, HALight* sender) {
    if (state != relayState) {
        pinMode(relayPin, OUTPUT);
        digitalWrite(relayPin, LOW);
        delay(200);
        pinMode(relayPin, INPUT_PULLUP);
        relayState = state;
    }

    this->entity.setState(state);
}

void Switch::setup() {
    // pinMode(relayPin, OUTPUT);
    // digitalWrite(relayPin, LOW);
    pinMode(relayPin, INPUT_PULLUP);

    entity.onStateCommand([this](bool state, HALight* sender) {
        this->onStateCommand(state, sender);
    });
}

void Switch::loop() {
    // TODO: Check if lamp is actually on
}
