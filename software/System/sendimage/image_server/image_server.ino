/*
 * GetImage
 * Matheus Gois Vieira
 * Abril de 2020
 */
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <SPIFFS.h>
#include <FS.h>
#include <EspCam.h>

//Nome do arquivo jpep
String name = "photo";

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

// Create object
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
EspCam cam(name);
DNSServer dns;
AsyncWebServer server(80);

//DEFINIÇÃO DE IP FIXO PARA O NODEMCU
IPAddress ip(192,168,1,222); //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR.
IPAddress gateway(192,168,0,1); //GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR.
IPAddress subnet(255,255,255,0); //MASCARA DE REDE.

void dh(){
  // Set btpClient
  timeClient.begin();
  timeClient.setTimeOffset(3600);

  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  
  formattedDate = timeClient.getFormattedDate();
  
  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.print(dayStamp);
  Serial.print(" ");
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  delay(1000);  
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);
  cam.ConfigEspCam();

  // Definindo IP Fixo
  WiFi.config(ip, gateway, subnet);
  
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

  camera_fb_t figure = cam.GetPhoto();
  Serial.print(" Pointer to the pixel data:");
  Serial.println(fb->buf);

  dh();   // See data/hours
}
void loop() {}
