#include "mqtt_app.h"

#include "esp_event.h"
#include "esp_log.h"
#include "gotham_protocol.h"
#include "mqtt_client.h"

static const char *TAG = "mqtt_app";
static esp_mqtt_client_handle_t s_client;
static volatile bool s_connected;
static mqtt_app_data_cb_t s_on_data;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    (void)handler_args;
    (void)base;
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        s_connected = true;
        ESP_LOGI(TAG, "MQTT connected to %s", CONFIG_BAT_MQTT_BROKER_URI);
        esp_mqtt_client_subscribe(event->client, GOTHAM_TOPIC_SIGNAL, 0);
        ESP_LOGI(TAG, "Subscribed to %s", GOTHAM_TOPIC_SIGNAL);
        break;

    case MQTT_EVENT_DISCONNECTED:
        s_connected = false;
        ESP_LOGW(TAG, "MQTT disconnected");
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT data topic=%.*s payload=%.*s",
                 event->topic_len, event->topic,
                 event->data_len, event->data);
        if (s_on_data) {
            s_on_data(event->data, event->data_len);
        }
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "MQTT error");
        break;

    default:
        break;
    }
}

void mqtt_app_start(mqtt_app_data_cb_t on_data)
{
    s_on_data = on_data;

    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BAT_MQTT_BROKER_URI,
    };

    s_client = esp_mqtt_client_init(&mqtt_cfg);
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(s_client, ESP_EVENT_ANY_ID,
                                                   mqtt_event_handler, NULL));
    ESP_ERROR_CHECK(esp_mqtt_client_start(s_client));
}

bool mqtt_app_is_connected(void)
{
    return s_connected && s_client != NULL;
}

int mqtt_app_publish(const char *topic, const char *payload)
{
    if (!mqtt_app_is_connected()) {
        return -1;
    }
    return esp_mqtt_client_publish(s_client, topic, payload, 0, 0, 0);
}
