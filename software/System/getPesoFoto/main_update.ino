#include <WiFi.h>
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
#include <EspCam.h>

// Defining Variables
char content;
boolean a = HIGH;
boolean takeNewPhoto = false;
String namePhoto = "/photo.jpg";
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
RegisterEsp reg("24:6F:28:15:71:4C, "http://contahub.com/iot.json");
EspCam cam;

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);

  cam.begin();

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
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", weight);
    response->addHeader("Access-Control-Allow-Origin", "http://sp.contahub.com");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    response->addHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Access-Control-Allow-Headers, Origin,Accept, X-Requested-With, Content-Type, Pragma, Cache-Control, Access-Control-Request-Method, Access-Control-Request-Headers");

    request->send(response);
  });

  server.on("/peso", HTTP_GET, [](AsyncWebServerRequest * request) {

    String weight = scale.readScale();
    flash.writeString(10, weight);

    WebSerial.println("Peso Capturado!");
    WebSerial.println("Peso:");
    WebSerial.println(weight);

    request->send(200, "text/html", weight);
    weight.remove(0);
  });

  server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, namePhoto, "image/jpg", false);
  });

  // Start server
  server.begin();

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WebSerial.print("ESP Board MAC Address:  ");
  WebSerial.println(WiFi.macAddress());

}

void loop() {
  if (takeNewPhoto) {
    cam.init();
    WebSerial.println("Take photo");
    cam.saveSpiffs(cam.getBuf(), cam.getBufLen(), namePhoto);
    WebSerial.print("The picture has been saved in ");
    WebSerial.print(namePhoto);
    takeNewPhoto = false;
  }
  delay(1);
}
