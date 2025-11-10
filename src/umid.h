#ifndef UMID_H
#define UMID_H

class Umid {
private:
    float umidade; // valor atual de umidade (%)
    bool ativo;

public:
    Umid();

    void ativar();
    void desativar();
    void setUmidade(float valor);
    float getUmidade() const;
    bool isAtivo() const;
};

#endif
