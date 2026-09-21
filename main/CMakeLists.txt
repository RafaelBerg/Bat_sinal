idf_component_register(SRCS "app_main.c"
                       PRIV_REQUIRES nvs_flash wifi_sta mqtt_app bat_led heartbeat gotham_protocol)
