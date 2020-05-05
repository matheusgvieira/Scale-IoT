/*
 * GetImage
 * Matheus Gois Vieira
 * Abril de 2020
 */

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <SPIFFS.h>
#include <FS.h>
#include <EspCam.h>

//Nome do arquivo jpep
String name = "photo";

// Create AsyncWebServer object on port 80
EspCam cam(name);
DNSServer dns;
AsyncWebServer server(80);

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);
  cam.ConfigEspCam();
  
  // Connect to Wi-Fi Manager
  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("Connected!");
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
    String photo = cam.GetHttpPhoto();
    Serial.print("Nome da foto:");
    Serial.println(photo);
    request->send(SPIFFS, photo, "image/jpg", false);
  });

  // Start server
  server.begin();

}

void loop() {}

