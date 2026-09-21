#pragma once

#include <stdbool.h>
#include <string.h>

#define GOTHAM_TOPIC_SIGNAL "gotham/dpgc/batsignal"
#define GOTHAM_TOPIC_STATUS "gotham/dpgc/status"
#define GOTHAM_MSG_SIGNAL_ON  "BAT_SIGNAL_ON"
#define GOTHAM_MSG_SIGNAL_OFF "BAT_SIGNAL_OFF"
#define GOTHAM_DEVICE_ID      "bat_sinal"
#define GOTHAM_HEARTBEAT_MS   30000

static inline bool gotham_payload_is(const char *data, int len, const char *msg)
{
    const int n = (int)strlen(msg);
    return data != NULL && msg != NULL && len == n && strncmp(data, msg, (size_t)n) == 0;
}
