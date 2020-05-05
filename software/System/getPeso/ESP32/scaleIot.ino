#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ScaleElgin.h>
#include <MemoryFlash.h>
#include <AsyncTCP.h>
#include <WebSerial.h>
#include <ArduinoJson.h>
#include <RegisterEsp.h>

//Variables
String weight;
const int led = 2;

// Create object e initialize function
MemoryFlash flash(512);
ScaleElgin scale;
DNSServer dns;
String ipToString(IPAddress ip);
void recvMsg(uint8_t *data, size_t len);
AsyncWebServer server(80);
void ConnectIpFixed();
RegisterEsp reg("24:6F:28:A1:9F:70", "http://contahub.com/iot.json");

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);

  //set pin led
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  // Serial port for debugging purposes
  Serial.println();
  Serial.println("Inicializando a Sistema");

  // Seach wifi e config
  ConnectIpFixed();

  // WebSerial is accessible at "<IP Address>/log" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();

  // Serial port tests WebSerial
  WebSerial.println("Inicializando...");
  WebSerial.println("Inicializando a Balança...");

  // Serial port tests
  Serial.println("WebSerial Inicializado");
  Serial.println("Inicializando a Balança...");


  server.on("/peso", HTTP_GET, [](AsyncWebServerRequest * request) {
    String weight = scale.readScale();
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", weight);

    // Datas for connection
    response->addHeader("Access-Control-Allow-Origin", "http://sp.contahub.com");
    response->addHeader("Access-Control-Allow-Credentials", "true");
    response->addHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Access-Control-Allow-Headers, Origin,Accept, X-Requested-With, Content-Type, Pragma, Cache-Control, Access-Control-Request-Method, Access-Control-Request-Headers");

    //Send response
    request->send(response);
  });

  server.on("/peso", HTTP_GET, [](AsyncWebServerRequest * request) {
    
    // Reading weight and saving in memory flash
    weight = scale.readScale();  
    flash.writeString(10, weight);
    digitalWrite(led, HIGH);
    
    //Print in webserial  
    WebSerial.println("Peso Capturado!");
    WebSerial.println("Peso:");
    WebSerial.println(weight);

    // Send content do peso
    request->send(200, "text/html", weight);
    digitalWrite(led, LOW);
  });

  // Start server
  server.begin();

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  WebSerial.print("ESP Board MAC Address:  ");
  WebSerial.println(WiFi.macAddress());

}

void loop() {}
