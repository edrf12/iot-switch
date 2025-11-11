---

# Documentação de Desenvolvimento

**Autor:** Lucas de Hollanda
**Data da Modificação:** 10/11/2025
**Branch:** `dev/hollanda`

---

## Resumo de Atividades

Durante esta atualização do sistema embarcado, foram implementadas classes específicas para controle de dispositivos IoT e integração com o protocolo MQTT utilizando **PlatformIO** e **ArduinoHA**.
As classes **AC**, **Lights** e **Umid** foram criadas para modularizar o controle de temperatura, luminosidade e umidade, respectivamente.

Principais mudanças:

* Criação dos arquivos `.cpp` e `.h` correspondentes a cada classe.
* Atualização do arquivo `main.cpp` para incluir chamadas às novas classes e seus métodos.
* Adição de registros MQTT:

  ```cpp
  mqtt.addDeviceType(&onboardLed);
  mqtt.addDeviceType(&transSwitch);
  ```

  permitindo o reconhecimento dos dispositivos pelo Home Assistant.
* Criação do arquivo `README.md` contendo instruções gerais sobre o projeto e orientações de uso.

Essas modificações tornaram o código mais modular, escalável e de fácil manutenção, melhorando a clareza e o reuso do código.

---

## Arquivos Criados e Alterados

### 1. Arquivos Criados

| Arquivo                   | Descrição                                                                                                             |
| ------------------------- | --------------------------------------------------------------------------------------------------------------------- |
| `ac.h` / `ac.cpp`         | Define e implementa a classe **AC**, responsável pelo controle de temperatura dos ar-condicionados.                   |
| `lights.h` / `lights.cpp` | Define e implementa a classe **Lights**, responsável pelo controle de intensidade e estado das lâmpadas inteligentes. |
| `umid.h` / `umid.cpp`     | Define e implementa a classe **Umid**, responsável pelo controle de umidade dos ar-condicionados.                     |
| `README.md`               | Adicionado para conter informações de configuração, uso e manutenção do sistema.                                      |

### 2. Arquivo Alterado

| Arquivo     | Alterações                                                                                                                                                                   |
| ----------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `main.cpp`  | Inclusão dos headers das novas classes, criação dos objetos (`ar`, `luz`, `umidade`), integração com o MQTT e adição de comandos no loop principal para controle via Serial. |
| `secrets.h` | Mantido apenas com definições de constantes e macros para credenciais Wi-Fi e MQTT (sem uso de getters e setters).                                                           |

---

## Funções Adicionadas

Abaixo estão listadas as novas funções criadas, indicando o arquivo e a classe correspondente.

---

### Classe **AC** (`ac.cpp` / `ac.h`)

Responsável por controlar o sistema de ar-condicionado.

**Métodos:**

* `void ligar()`
  Liga o ar-condicionado.
  Exibe no Serial: “Ar-condicionado ligado.”

* `void desligar()`
  Desliga o ar-condicionado.
  Exibe no Serial: “Ar-condicionado desligado.”

* `void setTemperatura(float temp)`
  Define a temperatura desejada.
  Exibe no Serial: “Temperatura ajustada para X°C.”

---

### Classe **Lights** (`lights.cpp` / `lights.h`)

Responsável pelo controle de lâmpadas inteligentes.

**Métodos:**

* `void ligar()`
  Liga a lâmpada.
  Exibe no Serial: “Lâmpada ligada.”

* `void desligar()`
  Desliga a lâmpada.
  Exibe no Serial: “Lâmpada desligada.”

* `void setIntensidade(int valor)`
  Ajusta a intensidade da luz (0 a 100).
  Exibe no Serial: “Intensidade da luz ajustada para X%.”

---

### Classe **Umid** (`umid.cpp` / `umid.h`)

Responsável pelo controle de umidade dos ar-condicionados.

**Métodos:**

* `void ativar()`
  Ativa o controle de umidade.
  Exibe no Serial: “Controle de umidade ativado.”

* `void desativar()`
  Desativa o controle de umidade.
  Exibe no Serial: “Controle de umidade desativado.”

* `void setUmidade(float valor)`
  Define o nível de umidade desejado.
  Exibe no Serial: “Umidade ajustada para X%.”

---

### Arquivo **main.cpp**

#### Função **setup()**

Responsável pela inicialização do sistema.

**Principais adições:**

* Inicialização da comunicação serial (`Serial.begin(115200)`).
* Conexão Wi-Fi utilizando credenciais definidas em `secrets.h`.
* Configuração MQTT com autenticação e *keep-alive*.
* Registro dos dispositivos MQTT:

  ```cpp
  mqtt.addDeviceType(&onboardLed);
  mqtt.addDeviceType(&transSwitch);
  ```
* Definição dos pinos físicos (`LED_BUILTIN`, `IRTX_PIN`).
* Inicialização dos objetos `AC`, `Lights` e `Umid`.

---

#### Função **loop()**

Função principal de execução contínua.

**Comportamento:**

* Chamada constante de `mqtt.loop()` para manter a comunicação MQTT ativa.
* Leitura de comandos via Serial (`Serial.readStringUntil('\n')`).
* Processamento dos comandos para acionar os métodos das classes:

  * `ac_on`, `ac_off`, `set_temp`
  * `light_on`, `light_off`, `set_light`
  * `umid_on`, `umid_off`, `set_umid`
* Exibição de mensagem `"Comando não reconhecido."` quando o comando não for identificado.

---

### Integração MQTT

Foram adicionados os seguintes registros:

```cpp
mqtt.addDeviceType(&onboardLed);
mqtt.addDeviceType(&transSwitch);
```

Esses registros garantem que o **Home Assistant** reconheça:

* O **LED onboard** como um dispositivo de luz controlável.
* O **transistor** como um switch digital MQTT.

---

## Conclusão

As modificações realizadas resultaram em:

* Estrutura modular com separação clara de responsabilidades;
* Melhor integração com o protocolo MQTT e o Home Assistant;
* Código mais organizado e legível;
* Base sólida para futuras expansões e adição de novos dispositivos.

---
