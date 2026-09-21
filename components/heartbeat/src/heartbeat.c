#include "heartbeat.h"

#include <stdio.h>

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gotham_protocol.h"
#include "mqtt_app.h"

static const char *TAG = "heartbeat";

static void heartbeat_task(void *arg)
{
    (void)arg;
    char payload[96];

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(GOTHAM_HEARTBEAT_MS));
        if (!mqtt_app_is_connected()) {
            continue;
        }

        const int64_t uptime_s = esp_timer_get_time() / 1000000LL;
        snprintf(payload, sizeof(payload),
                 "{\"device\": \"%s\", \"status\": \"ONLINE\", \"uptime_s\": %lld}",
                 GOTHAM_DEVICE_ID, (long long)uptime_s);

        if (mqtt_app_publish(GOTHAM_TOPIC_STATUS, payload) < 0) {
            ESP_LOGW(TAG, "Heartbeat publish failed");
        } else {
            ESP_LOGI(TAG, "Heartbeat %s", payload);
        }
    }
}

void heartbeat_start(void)
{
    xTaskCreate(heartbeat_task, "heartbeat", 3072, NULL, 5, NULL);
}
