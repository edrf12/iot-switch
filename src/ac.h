#ifndef AC_H
#define AC_H

class AC {
private:
    float temperatura;  // temperatura atual do ar-condicionado
    bool ligado;

public:
    AC();  // construtor

    void ligar();
    void desligar();
    void setTemperatura(float novaTemp);
    float getTemperatura() const;
    bool isLigado() const;
};

#endif
