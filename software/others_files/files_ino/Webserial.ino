#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

AsyncWebServer server(80);

const char* ssid = "Familia_de_Deus"; // Your WiFi SSID
const char* password = "gois6523"; // Your WiFi Password
char value = 'c';


void recvMsg(uint8_t *data, size_t len) {
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);
}

void setup() {

  Serial.begin(115200);
  pinMode(2,OUTPUT);
  Serial.println("Conectando...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  Serial.println("Iniciando Teste do WebSerial");
}

void loop() {
  WebSerial.println("http://192.168.1.135");
  WebSerial.println("Captured Photo");
  WebSerial.println("Peso: 195kg");
  WebSerial.println("");
  
  delay(3000);
}
