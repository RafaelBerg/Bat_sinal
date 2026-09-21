#include "bat_led.h"

#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "bat_led";

void bat_led_init(void)
{
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << CONFIG_BAT_LED_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&io));
    ESP_ERROR_CHECK(gpio_set_level(CONFIG_BAT_LED_GPIO, 0));
    ESP_LOGI(TAG, "LED on GPIO %d (active high, off)", CONFIG_BAT_LED_GPIO);
}

void bat_led_set(bool on)
{
    ESP_ERROR_CHECK(gpio_set_level(CONFIG_BAT_LED_GPIO, on ? 1 : 0));
    if (on) {
        ESP_LOGI(TAG, "[ALERTA] Bat-Sinal Ativado! O Cavaleiro das Trevas foi convocado.");
    } else {
        ESP_LOGI(TAG, "[INFO] Bat-Sinal Desativado.");
    }
}
