#ifndef _HTTP_CLI_H_
#define _HTTP_CLI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_camera.h"
#include "esp_err.h"

esp_err_t async_frame_post(camera_fb_t *frame);

#ifdef __cplusplus
}
#endif

#endif