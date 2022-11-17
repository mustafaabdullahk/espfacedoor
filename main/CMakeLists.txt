# idf_component_register(SRCS "httpcli.c" "camerahelper.c" "wifihelper.c" "main.c"
#                     INCLUDE_DIRS ".")
idf_component_register(SRC_DIRS .
                       INCLUDE_DIRS .
                       REQUIRES esp32-camera esp_http_client nvs_flash)
