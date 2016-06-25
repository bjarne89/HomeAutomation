/* Extremely simple example that just reads from stdin and echoes back on stdout
 *
 * Has an easter egg, which is if you type "QUACK" it'll quack 3 times back at you.
 *
 * This example code is in the public domain
 */
#include "espressif/esp_common.h"
#include <esp/uart.h>
#include <string.h>

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
    printf("Going into echo mode...\n");
}
