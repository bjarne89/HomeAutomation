#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "esp8266.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ssid_config.h"
#include "esp/spi.h"

#include <esp/uart.h>
#include <string.h>

#define COL5 5
#define LATCHPIN 13

blinkTask(void *pvParameters)
{

    gpio_enable(COL5, GPIO_OUTPUT);
    //gpio_enable(LATCHPIN, GPIO_OUTPUT);

    /*printf("taskstart ok\n");
    spi_init(0, SPI_MODE0, SPI_FREQ_DIV_1M, true, SPI_LITTLE_ENDIAN, false);
    printf("SPI init ok\n");


    uint8_t send = 0b10101010;
    gpio_write(LATCHPIN, 1);
    spi_transfer_8(0, send);
    gpio_write(LATCHPIN, 0);
    printf("SPI transfer ok\n");*/

    while(1) {
        gpio_write(COL5, 1);
        
        printf("ON\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
        gpio_write(COL5, 0);
        
        printf("OFF\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
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

    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());


    xTaskCreate(blinkTask, (signed char *)"blinkTask", 256, NULL, 2, NULL);
}