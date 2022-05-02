#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#define STA_SSID "Tushar's iPhone"
#define STA_PASSWORD "1234567890"

void scann()
{

    wifi_scan_config_t scan_config = {
        .ssid = 0,
        .bssid = 0,
        .channel = 0,
        .show_hidden = true};
    printf("Start scanning...");
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
    printf(" completed!\n");

    uint16_t ap_num;
    wifi_ap_record_t ap_records[20];
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_num));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));

    printf("Found %d access points:\n", ap_num);

    printf("               SSID              | Channel | RSSI |   MAC \n\n");
    for (int i = 0; i < ap_num; i++)
        printf("%32s | %7d | %4d   %2x:%2x:%2x:%2x:%2x:%2x   \n", ap_records[i].ssid, ap_records[i].primary, ap_records[i].rssi, *ap_records[i].bssid, *(ap_records[i].bssid + 1), *(ap_records[i].bssid + 2), *(ap_records[i].bssid + 3), *(ap_records[i].bssid + 4), *(ap_records[i].bssid + 5));
}

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t sta_config = {
        .sta = {
            .ssid = STA_SSID,
            .password = STA_PASSWORD},
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));

    ESP_ERROR_CHECK(esp_wifi_start());
    // ESP_ERROR_CHECK(esp_wifi_connect());
    esp_err_t check = esp_wifi_connect();
    if (check == ESP_OK)
    {
        printf("Connected!!\n");
    }

    while (1)
    {
        vTaskDelay(3000 / portTICK_RATE_MS);
        scann();
    }
}