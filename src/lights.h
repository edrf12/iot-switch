#ifndef LIGHTS_H
#define LIGHTS_H

class Lights {
private:
    int intensidade; // brilho de 0 a 100
    bool ligada;

public:
    Lights();

    void ligar();
    void desligar();
    void setIntensidade(int valor);
    int getIntensidade() const;
    bool isLigada() const;
};

#endif
