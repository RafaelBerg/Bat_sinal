#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*mqtt_app_data_cb_t)(const char *data, int len);

void mqtt_app_start(mqtt_app_data_cb_t on_data);
bool mqtt_app_is_connected(void);
int mqtt_app_publish(const char *topic, const char *payload);

#ifdef __cplusplus
}
#endif
