/*
 * Biblioteca para ESP32-CAM
 * Autor: Matheus Gois Vieira
 * Data: Abril de 2020
 */

#ifndef ESPCAM_H_INCLUDED
#define ESPCAM_H_INCLUDED

#include <Arduino.h>
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"          // Disable brownour problems
#include "soc/rtc_cntl_reg.h" // Disable brownour problems
#include "driver/rtc_io.h"
#include <SPIFFS.h>
#include <FS.h>
#include "sys/time.h"

class EspCam
{
public:
    EspCam();
    void begin();
    uint8_t * getBuf();
    size_t getBufLen();
    size_t getWidth();
    size_t getHeight();
    pixformat_t getFormatImage();
    int getTimeStamp();
    void saveSpiffs(uint8_t * buf, size_t len, String nameFoto);
    void init();

private:
    String _namePhoto;
    uint8_t *_bufImage;
    size_t _bufLen;
    size_t _width;
    size_t _height;
    pixformat_t _formatImage;
    int _time;
};

#endif