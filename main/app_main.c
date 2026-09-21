#include "bat_led.h"
#include "esp_err.h"
#include "gotham_protocol.h"
#include "heartbeat.h"
#include "mqtt_app.h"
#include "nvs_flash.h"
#include "wifi_sta.h"

static void on_batsignal(const char *data, int len)
{
    if (gotham_payload_is(data, len, GOTHAM_MSG_SIGNAL_ON)) {
        bat_led_set(true);
    } else if (gotham_payload_is(data, len, GOTHAM_MSG_SIGNAL_OFF)) {
        bat_led_set(false);
    }
}

void app_main(void)
{
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
