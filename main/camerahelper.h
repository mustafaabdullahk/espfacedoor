#ifndef _CAMERA_HELPER_H_
#define _CAMERA_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include "sensor.h"

esp_err_t init_camera(uint32_t xclk_freq_hz, pixformat_t pixel_format, framesize_t frame_size, uint8_t fb_count);

#ifdef __cplusplus
}
#endif

#endif