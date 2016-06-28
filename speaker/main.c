#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pwm.h"
#include "ssid_config.h"

#define SPEAKER 12

void task1(void *pvParameters)
{
    uint32_t const init_count = 20;
    uint32_t count = init_count;
    while(1) {
        vTaskDelay(100);
        printf("duty cycle set to %d/UINT16_MAX%%\r\n", count);
        pwm_set_freq(count);
        count += 400;
        if (count > 20000) count = init_count;
    }
}

void user_init(void)
{
    struct ip_info ipInfo;
    IP4_ADDR(&ipInfo.ip, 10, 0, 0, 101);
    IP4_ADDR(&ipInfo.gw, 10, 0, 0, 1);
    IP4_ADDR(&ipInfo.netmask, 255, 255, 255, 0);
    sdk_wifi_set_ip_info(STATION_IF, &ipInfo);

    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);

    uint8_t pins[1];
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    pins[0] = SPEAKER;
    pwm_init(1, pins);
    pwm_set_freq(1000);
    pwm_set_duty(UINT16_MAX/100);
    pwm_start();

    xTaskCreate(task1, (signed char *)"tsk1", 256, NULL, 2, NULL);
}
