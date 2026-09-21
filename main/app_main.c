#include "bat_led.h"
#include "esp_err.h"
#include "esp_log.h"
#include "gotham_protocol.h"
#include "heartbeat.h"
#include "mqtt_app.h"
#include "nvs_flash.h"
#include "wifi_sta.h"

static const char *TAG = "bat_sinal";

static void on_batsignal(const char *data, int len)
{
    if (gotham_payload_is(data, len, GOTHAM_MSG_SIGNAL_ON)) {
        bat_led_set(true);
    } else if (gotham_payload_is(data, len, GOTHAM_MSG_SIGNAL_OFF)) {
        bat_led_set(false);
    } else {
        ESP_LOGW(TAG, "Payload desconhecido em %s", GOTHAM_TOPIC_SIGNAL);
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Bat-Sinal Node B | target=esp32s3 | LED GPIO=%d | broker=%s",
             CONFIG_BAT_LED_GPIO, CONFIG_BAT_MQTT_BROKER_URI);
    if (CONFIG_BAT_WIFI_SSID[0] == '\0') {
        ESP_LOGE(TAG, "Wi-Fi SSID vazio! Rode: idf.py menuconfig -> Bat-Sinal Configuration");
    } else {
        ESP_LOGI(TAG, "Wi-Fi SSID: %s", CONFIG_BAT_WIFI_SSID);
    }

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    bat_led_init();
    wifi_sta_start();
    mqtt_app_start(on_batsignal);
    heartbeat_start();
}
