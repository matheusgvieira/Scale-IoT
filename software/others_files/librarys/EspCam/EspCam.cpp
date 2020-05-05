/*
 * Biblioteca para ESP32-CAM
 * Autor: Matheus Gois Vieira
 * Data: Abril de 2020
 */

#include "EspCam.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"          // Disable brownour problems
#include "soc/rtc_cntl_reg.h" // Disable brownour problems
#include "driver/rtc_io.h"
#include <SPIFFS.h>
#include <FS.h>

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

EspCam::EspCam()
{
}

void EspCam::begin()
{
  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else
  {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }

  // OV2640 camera module
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  }
  else
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
}

// Check if photo capture was successful
// bool checkPhoto( fs::FS &fs ) {
//   File f_pic = fs.open(FILE_PHOTO);
//   unsigned int pic_sz = f_pic.size();
//   return (pic_sz > 100);
// }

void EspCam::saveSpiffs(uint8_t *buf, size_t len, String nameFoto)
{
  while (true)
  {
    Serial.print(".");
    
    // Photo file name
    //Serial.printf("Picture file name: %s\n", nameFoto.c_str());
    File file = SPIFFS.open(nameFoto, FILE_WRITE);

    // Insert the data in the photo file
    if (!file)
    {
      Serial.println("Failed to open file in writing mode");
    }
    else
    {
      file.write(buf, len); // payload (image), payload length
      if (file.size() != 0)
      {
        Serial.println("");
        Serial.print("The picture has been saved in ");
        Serial.println(nameFoto);
        Serial.print("Tamanho do arquivo: ");
        Serial.println(file.size());
        Serial.print("Time: ");
        Serial.println(getTimeStamp());
        Serial.println("");
        break;
      }
    }
  }
}

// Capture Photo and Save it to SPIFFS
void EspCam::init(void)
{
  camera_fb_t *fb = NULL; // pointer
  fb = esp_camera_fb_get();
  if (!fb)
  {
    Serial.println("Camera capture failed");
    return;
  }
  else
  {
    _bufImage = fb->buf;
    _bufLen = fb->len;
    _width = fb->width;
    _height = fb->height;
    _formatImage = fb->format;
    _time = millis();
    Serial.println("Take Photo!");
  }
}

uint8_t *EspCam::getBuf()
{
  return _bufImage;
}
size_t EspCam::getBufLen()
{
  return _bufLen;
}
size_t EspCam::getWidth()
{
  return _width;
}
size_t EspCam::getHeight()
{
  return _height;
}
pixformat_t EspCam::getFormatImage()
{
  return _formatImage;
}
int EspCam::getTimeStamp()
{
  return _time;
}