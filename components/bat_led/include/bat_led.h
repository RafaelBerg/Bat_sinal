#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void bat_led_init(void);
void bat_led_set(bool on);

#ifdef __cplusplus
}
#endif
