#include "ac.h"
#include <Arduino.h>

AC::AC() : temperatura(24.0), ligado(false) {}

void AC::ligar() {
    ligado = true;
    Serial.println("Ar-condicionado ligado.");
}

void AC::desligar() {
    ligado = false;
    Serial.println("Ar-condicionado desligado.");
}

void AC::setTemperatura(float novaTemp) {
    if (ligado) {
        temperatura = novaTemp;
        Serial.print("Temperatura ajustada para: ");
        Serial.println(temperatura);
    } else {
        Serial.println("Erro: AC desligado, não é possível ajustar temperatura.");
    }
}

float AC::getTemperatura() const {
    return temperatura;
}

bool AC::isLigado() const {
    return ligado;
}
