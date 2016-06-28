#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "esp8266.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ssid_config.h"
#include "esp/spi.h"

#include <esp/uart.h>
#include <string.h>

#define COL1 0
#define COL2 3 //Rx
#define COL3 12
#define COL4 2
#define COL5 4
#define LATCHPIN 5
//CLK = pin 14
//DATA = pin 13

blinkTask(void *pvParameters)
{
    uint16_t send1 = 0b1110000000000000;
    uint16_t send2 = 0b0000111000000000;
    uint16_t send3 = 0b0000000111000000;
    uint16_t send4 = 0b0000000000111000;
    uint16_t send5 = 0b0000000000000111;
    uint16_t test = 100;

    while(1) {
        gpio_write(COL5, 0);
        gpio_write(LATCHPIN, 0);
        spi_transfer_16(1, send1);
        gpio_write(LATCHPIN, 1);
        gpio_write(COL1, 1);
        vTaskDelay(6*test / portTICK_RATE_MS);

        gpio_write(COL1, 0);
        gpio_write(LATCHPIN, 0);
        spi_transfer_16(1, send2);
        gpio_write(LATCHPIN, 1);
        gpio_write(COL2, 1);
        vTaskDelay(6*test / portTICK_RATE_MS);

        gpio_write(COL2, 0);
        gpio_write(LATCHPIN, 0);
        spi_transfer_16(1, send3);
        gpio_write(LATCHPIN, 1);
        gpio_write(COL3, 1);
        vTaskDelay(6*test / portTICK_RATE_MS);

        gpio_write(COL3, 0);
        gpio_write(LATCHPIN, 0);
        spi_transfer_16(1, send4);
        gpio_write(LATCHPIN, 1);
        gpio_write(COL4, 1);
        vTaskDelay(6*test / portTICK_RATE_MS);

        gpio_write(COL4, 0);
        gpio_write(LATCHPIN, 0);
        spi_transfer_16(1, send5);
        gpio_write(LATCHPIN, 1);
        gpio_write(COL5, 1);
        vTaskDelay(6*test / portTICK_RATE_MS);
    }
}

void user_init(void)
{
    struct ip_info ipInfo;
    IP4_ADDR(&ipInfo.ip, 10, 0, 0, 100);
    IP4_ADDR(&ipInfo.gw, 10, 0, 0, 1);
    IP4_ADDR(&ipInfo.netmask, 255, 255, 255, 0);
    sdk_wifi_set_ip_info(STATION_IF, &ipInfo);

    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);


    spi_init(1, SPI_MODE0, SPI_FREQ_DIV_1M, true, SPI_BIG_ENDIAN, true);

    gpio_enable(COL1, GPIO_OUTPUT);
    gpio_enable(COL2, GPIO_OUTPUT);
    gpio_enable(COL3, GPIO_OUTPUT);
    gpio_enable(COL4, GPIO_OUTPUT);
    gpio_enable(COL5, GPIO_OUTPUT);
    gpio_enable(LATCHPIN, GPIO_OUTPUT);
    gpio_write(COL1, 0);
    gpio_write(COL2, 0);
    gpio_write(COL3, 0);
    gpio_write(COL4, 0);
    gpio_write(COL5, 0);
    gpio_write(LATCHPIN, 1);

    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());


    xTaskCreate(blinkTask, (signed char *)"blinkTask", 256, NULL, 2, NULL);
}