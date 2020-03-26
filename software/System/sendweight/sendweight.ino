#include <WiFi.h>

const char* ssid = "Familia_de_Deus";
const char* pass = "gois6523";

const uint16_t port = 8090;
const char * host = "192.168.1.106";

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
  Serial.print("Wifi connected with IP: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection to host failed");

    delay(1000);
    return;
  }

  Serial.println("Connected to server successful!");

  Serial.print("Peso:");
  Serial.println("310kg");
  client.println("310kg");

  if (client.available() > 0) {
    String c = client.readString();
    Serial.println(c);
  }

  delay(10);

  Serial.println("Disconnecting...");
  client.stop();

  delay(10000);

}
