#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

#define core0 0
#define core1 1

#define PIN 4
static const char *TAG = "LED STATUS";

void core1_led_blink(void *pvParameter)
{
    esp_rom_gpio_pad_select_gpio(PIN);
    esp_log_level_set("LOG", ESP_LOG_INFO);
    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
    int isON = 0;

    while (true)
    {
        gpio_set_level(PIN, isON);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "Turning the LED %s!", isON == true ? "ON" : "OFF");
        isON = !isON;
        fflush(stdout);
    }
}

void core0_hello(void *pvParameter)
{
    while (1)
    {
        printf("Hello world from core %d!\n", xPortGetCoreID());
        vTaskDelay(1323 / portTICK_PERIOD_MS);
        fflush(stdout);
    }
}

void app_main()
{
    nvs_flash_init();
    xTaskCreatePinnedToCore(&core0_hello, "core1_task", 1024 * 4, NULL, configMAX_PRIORITIES - 1, NULL, core0);
    xTaskCreatePinnedToCore(&core1_led_blink, "core0_task", 1024 * 4, NULL, configMAX_PRIORITIES - 1, NULL, core1);
}