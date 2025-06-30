# 🛠️ Scanner I2C Universal para BitDogLab com Raspberry Pi Pico

![Linguagem](https://img.shields.io/badge/Linguagem-C-blue.svg)
![Plataforma](https://img.shields.io/badge/Plataforma-Raspberry%20Pi%20Pico-purple.svg)
![Ferramenta](https://img.shields.io/badge/Ferramenta-Diagnóstico-lightgrey.svg)

O programa, desenvolvido em C com o Pico SDK, escaneia continuamente os dois barramentos I2C da placa (I2C 0 e I2C 1) e exibe em tempo real no monitor serial todos os dispositivos conectados.

A principal vantagem é a capacidade de "hot-swap": o usuário pode conectar e desconectar sensores com a placa ligada, e o scanner irá detectar as mudanças no ciclo seguinte, tornando o processo de teste de hardware extremamente ágil e eficiente.

## ✨ Funcionalidades Principais

* **Scan Duplo:** Varre simultaneamente os barramentos **I2C 0 (pinos 0, 1)** e **I2C 1 (pinos 2, 3)**.
* **Identificação Inteligente:** Utiliza um banco de dados interno para identificar dispositivos conhecidos pelo nome, em vez de apenas mostrar seus endereços hexadecimais.
* **Monitoramento Contínuo:** Repete o scan a cada 10 segundos, permitindo a troca de sensores sem a necessidade de reiniciar a placa.
* **Código Reutilizável:** A lógica de scan é encapsulada em uma função modular, facilitando a manutenção e a compreensão do código.
* **Detecção de Dispositivos Integrados:** O scanner é capaz de identificar componentes I2C que já vêm soldados na placa, como o misterioso sensor no endereço `0x6B`.

## 💻 Exemplo de Saída no Terminal

A saída no monitor serial é clara e organizada, mostrando os resultados de cada barramento separadamente.

==================== SCAN #1 ====================

--- Escaneando Barramento I2C 0 (Direita) ---
Pinos: SDA=0, SCL=1

Endereço 0x6B: Dispositivo Integrado na Placa (Provavelmente IMU no I2C 0)

--- Escaneando Barramento I2C 1 (Esquerda) ---
Pinos: SDA=2, SCL=3

Endereço 0x23: Sensor de Luminosidade BH1750

--- Fim do Scan. Aguardando 10 segundos... ---

## 🛠️ Hardware e Software Necessários

### Hardware
* Placa **BitDogLab**
* Sensores I2C diversos para teste

### Software
* [Raspberry Pi Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk)
* Um ambiente de compilação C/C++ (GCC para ARM, CMake, etc.)
* Um programa para monitor serial (ex: o integrado no VS Code, PuTTY, etc.)

## 🚀 Como Compilar e Usar

1.  **Clone o Repositório:**
    ```bash
    git clone [URL_DO_SEU_REPOSITÓRIO_AQUI]
    cd nome-do-repositorio
    ```

2.  **Configure o Ambiente de Build:**
    * Certifique-se de que o Pico C/C++ SDK está instalado e que a variável de ambiente `PICO_SDK_PATH` está definida.

3.  **Compile o Firmware:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4.  **Grave o Firmware:**
    * Pressione e segure o botão `BOOTSEL` na sua placa e conecte-a ao computador.
    * Ela aparecerá como um dispositivo de armazenamento USB.
    * Arraste e solte o arquivo `.uf2` (que está na pasta `build`) para dentro do dispositivo.

5.  **Teste:**
    * Abra um monitor serial conectado à porta COM do Pico.
    * O programa começará a exibir os resultados do scan a cada 10 segundos.
    * Conecte e desconecte seus sensores nas portas I2C 0 (direita) e I2C 1 (esquerda) e veja os resultados mudarem em tempo real.

## 📂 Estrutura dos Arquivos

* **main.c:** Contém toda a lógica do programa, incluindo o banco de dados de dispositivos, a função de scan e o loop principal de monitoramento.
* **CMakeLists.txt:** Arquivo de build do projeto.

## ✍️ Autor

[ASCCJR]
