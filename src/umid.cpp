#include "umid.h"
#include <Arduino.h>

Umid::Umid() : umidade(50.0), ativo(false) {}

void Umid::ativar() {
    ativo = true;
    Serial.println("Controle de umidade ativado.");
}

void Umid::desativar() {
    ativo = false;
    Serial.println("Controle de umidade desativado.");
}

void Umid::setUmidade(float valor) {
    if (ativo) {
        umidade = valor;
        Serial.print("Umidade ajustada para: ");
        Serial.print(umidade);
        Serial.println("%");
    } else {
        Serial.println("Erro: controle de umidade desativado.");
    }
}

float Umid::getUmidade() const {
    return umidade;
}

bool Umid::isAtivo() const {
    return ativo;
}
