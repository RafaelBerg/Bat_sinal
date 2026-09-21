idf_component_register(SRCS "src/mqtt_app.c"
                       INCLUDE_DIRS "include"
                       REQUIRES gotham_protocol
                       PRIV_REQUIRES esp_event)
