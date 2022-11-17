#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "camerahelper.h"
#include "wifihelper.h"
#include "httpcli.h"

#include "esp_camera.h"
#include "esp_log.h"

#define TEST_ESP_OK(ret) assert(ret == ESP_OK)
#define TEST_ASSERT_NOT_NULL(ret) assert(ret != NULL)

static const char *TAG = "ESP STREAM SERVER";

void app_main(void)
{
    wifi_init();
    camera_fb_t *frame;

     /* It is recommended to use a camera sensor with JPEG compression to maximize the speed */
    TEST_ESP_OK(init_camera(5000000, PIXFORMAT_JPEG, FRAMESIZE_QVGA, 1));

    ESP_LOGI(TAG, "Begin capture frame");
    
    while (true) {

        frame = esp_camera_fb_get();

        if (frame) {
            // xQueueSend(xQueueIFrame, &frame, portMAX_DELAY);
            async_frame_post(frame);
            vTaskDelay(1000);
        } 
    }
}
