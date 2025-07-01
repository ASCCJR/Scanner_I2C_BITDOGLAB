/**
 * @file main.c
 * @brief Scanner I2C Universal para a placa BitDogLab.
 *
 * Este programa escaneia continuamente os barramentos I2C 0 e I2C 1,
 * imprimindo no monitor serial os dispositivos encontrados e identificando
 * aqueles que estão em seu banco de dados interno.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string.h>

// --- Banco de Dados de Dispositivos I2C Conhecidos ---
typedef struct {
    uint8_t addr;
    const char *name;
} I2CDevice;

const I2CDevice known_devices[] = {
    {0x23, "Sensor de Luminosidade BH1750"},
    {0x29, "Sensor de Distancia VL53L0X OU Sensor de Cor TCS34725"},
    {0x3C, "Display OLED SSD1306/SH1106 (no I2C 1)"},
    {0x68, "Acelerometro/Giroscopio MPU-6500/9250 (AD0 em GND)"},
    {0x69, "Acelerometro/Giroscopio MPU-6500/9250 (AD0 em VCC)"},
    {0x76, "Sensor de Pressao/Temp BMP280 (SDO em GND)"},
    {0x38, "Sensor de Umidade e Temperatura AHT10"},
    {0x57, "Oximetro de Pulso / Batimentos Cardiacos MAX30100"},
    {0x6B, "Dispositivo Integrado na Placa (Provavelmente chip carregador de bateria que está conectado na i2c-0)"}
};
const int num_known_devices = sizeof(known_devices) / sizeof(known_devices[0]);


/**
 * @brief Escaneia um barramento I2C e imprime os dispositivos encontrados.
 * @param i2c_port A instância do I2C a ser usada (i2c0 ou i2c1).
 * @param sda_pin O pino GPIO para o SDA.
 * @param scl_pin O pino GPIO para o SCL.
 * @param port_name Uma string com o nome do barramento para impressão.
 */
void scan_i2c_bus(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin, const char* port_name) {
    printf("\n--- Escaneando Barramento %s ---\n", port_name);
    printf("Pinos: SDA=%d, SCL=%d\n", sda_pin, scl_pin);

    // Inicializa o barramento I2C específico com os pinos corretos.
    i2c_init(i2c_port, 100 * 1000);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    int found_count = 0;

    // Loop para escanear todos os endereços I2C válidos.
    for (int addr = 0x08; addr < 0x78; ++addr) {
        uint8_t rxdata;
        int ret = i2c_read_blocking(i2c_port, addr, &rxdata, 1, false);
        if (ret >= 0) {
            bool is_known = false;
            // Verifica se o endereço encontrado está no banco de dados.
            for (int j = 0; j < num_known_devices; j++) {
                if (addr == known_devices[j].addr) {
                    printf(">> Endereço 0x%02X: %s\n", addr, known_devices[j].name);
                    is_known = true;
                    break;
                }
            }
            if (!is_known) {
                printf(">> Endereço 0x%02X: Dispositivo desconhecido\n", addr);
            }
            found_count++;
        }
    }

    if (found_count == 0) {
        printf("Nenhum dispositivo encontrado neste barramento.\n");
    }

    // Desinicializa os pinos para evitar conflitos.
    gpio_disable_pulls(sda_pin);
    gpio_disable_pulls(scl_pin);
    gpio_set_function(sda_pin, GPIO_FUNC_NULL);
    gpio_set_function(scl_pin, GPIO_FUNC_NULL);
}


/**
 * @brief Função principal do programa.
 */
int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    int scan_count = 1;

    // Loop principal para scan contínuo.
    while(true) {
        printf("\n==================== SCAN #%d ====================\n", scan_count++);

        // Chama a função de scan para a porta I2C 0.
        scan_i2c_bus(i2c0, 0, 1, "I2C 0 (Direita)");

        // Chama a função de scan para a porta I2C 1.
        scan_i2c_bus(i2c1, 2, 3, "I2C 1 (Esquerda)");

        printf("\n--- Fim do Scan. Aguardando 10 segundos... ---\n");

        // Pausa de 10 segundos antes do próximo ciclo.
        sleep_ms(10000);
    }

    return 0; // Esta linha nunca será alcançada.
}
