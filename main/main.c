#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "camerahelper.h"
#include "wifihelper.h"
#include "httpcli.h"


#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_camera.h"
#include "esp_log.h"

#include "wifi_manager.h"

#define TEST_ESP_OK(ret) assert(ret == ESP_OK)
#define TEST_ASSERT_NOT_NULL(ret) assert(ret != NULL)

static const char *TAG = "ESP STREAM SERVER";

/**
 * @brief RTOS task that periodically prints the heap memory available.
 * @note Pure debug information, should not be ever started on production code! This is an example on how you can integrate your code with wifi-manager
 */
void monitoring_task(void *pvParameter)
{
	for(;;){
		ESP_LOGI(TAG, "free heap: %d",esp_get_free_heap_size());
		vTaskDelay( pdMS_TO_TICKS(10000) );
	}
}

/**
 * @brief this is an exemple of a callback that you can setup in your own app to get notified of wifi manager event.
 */
void cb_connection_ok(void *pvParameter){
	ip_event_got_ip_t* param = (ip_event_got_ip_t*)pvParameter;

	/* transform IP to human readable string */
	char str_ip[16];
	esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, IP4ADDR_STRLEN_MAX);

	ESP_LOGI(TAG, "I have a connection and my IP is %s!", str_ip);

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

void app_main(void)
{
    // wifi_init();

    /* start the wifi manager */
    wifi_manager_start();

    /* register a callback as an example to how you can integrate your code with the wifi manager */
    wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);

    /* your code should go here. Here we simply create a task on core 2 that monitors free heap memory */
	xTaskCreatePinnedToCore(&monitoring_task, "monitoring_task", 2048, NULL, 1, NULL, 1);
}
