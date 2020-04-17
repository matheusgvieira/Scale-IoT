#include <WiFi.h>
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ScaleElgin.h>
#include <MemoryFlash.h>
#include <ESPAsyncWiFiManager.h>
#include <AsyncTCP.h>
#include <WebSerial.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <RegisterEsp.h>

// Defining Variables
char content;
boolean a = HIGH;
boolean takeNewPhoto = false;
String peso;

// Create object e initialize function
MemoryFlash flash(1);
ScaleElgin scale(peso);
DNSServer dns;
String ipToString(IPAddress ip);
void recvMsg(uint8_t *data, size_t len);
String czero(String json);
AsyncWebServer server(80);
void ConnectIpFixed();
RegisterEsp reg("http://contahub.com/iot.json");


// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/photo.jpg"

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);

  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // Serial port for debugging purposes
  Serial.println();
  Serial.println("Inicializando a Sistema");

  // Seach wifi e config
  ConnectIpFixed();
  reg.begin();

  // Config SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }

  // Print ESP32 Local IP Address
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  // WebSerial is accessible at "<IP Address>/log" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  Serial.println("WebSerial Inicializado");
  WebSerial.println("Inicializando...");

  // Serial port tests
  Serial.println();
  Serial.println("Inicializando a Balança...");
  WebSerial.println("Inicializando a Balança...");

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

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }

  // Route for root / web page
  // Route for root / web page
  server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/dashboard.html", "text/html");
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/action.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/action.js", "text/js");
  });

  server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
    takeNewPhoto = true;
    request->send_P(200, "text/plain", "Taking Photo");
  });

  server.on("/peso", HTTP_GET, [](AsyncWebServerRequest * request) {
    String weight = scale.readScale();
    weight = czero(weight);
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", weight);
    response->addHeader("Access-Control-Allow-Origin", "http://sp.contahub.com");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    response->addHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Access-Control-Allow-Headers, Origin,Accept, X-Requested-With, Content-Type, Pragma, Cache-Control, Access-Control-Request-Method, Access-Control-Request-Headers");

    request->send(response);
  });

  server.on("/peso", HTTP_GET, [](AsyncWebServerRequest * request) {

    String weight = scale.readScale();
    weight = czero(weight);
    flash.writeString(10, weight);

    WebSerial.println("Peso Capturado!");
    WebSerial.println("Peso:");
    WebSerial.println(weight);

    request->send(200, "text/html", weight);
  });

  server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, FILE_PHOTO, "image/jpg", false);
  });

  // Start server
  server.begin();

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WebSerial.print("ESP Board MAC Address:  ");
  WebSerial.println(WiFi.macAddress());

}

void capturePhotoSaveSpiffs();

void loop() {
  if (takeNewPhoto) {
    capturePhotoSaveSpiffs();
    takeNewPhoto = false;
  }
  delay(1);
}
