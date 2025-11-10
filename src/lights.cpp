#include "lights.h"
#include <Arduino.h>

Lights::Lights() : intensidade(0), ligada(false) {}

void Lights::ligar() {
    ligada = true;
    intensidade = 50; // intensidade padrão
    Serial.println("Lâmpada ligada.");
}

void Lights::desligar() {
    ligada = false;
    Serial.println("Lâmpada desligada.");
}

void Lights::setIntensidade(int valor) {
    if (ligada) {
        if (valor >= 0 && valor <= 100) {
            intensidade = valor;
            Serial.print("Intensidade ajustada para: ");
            Serial.println(intensidade);
        } else {
            Serial.println("Valor inválido. Use de 0 a 100.");
        }
    } else {
        Serial.println("Erro: Lâmpada desligada, não é possível ajustar intensidade.");
    }
}

int Lights::getIntensidade() const {
    return intensidade;
}

bool Lights::isLigada() const {
    return ligada;
}
