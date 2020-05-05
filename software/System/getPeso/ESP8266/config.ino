void recvMsg(uint8_t *data, size_t len) {
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);
}

String ipToString(IPAddress ip) {
  String s = "";
  for (int i = 0; i < 4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

void ConnectIpFixed() {
  IPAddress numGat;
  IPAddress numSub;
  int numIp[2];

  // Wait for connection
  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.autoConnect("EspContaHUB", "contahub");
  Serial.println("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  numGat = WiFi.gatewayIP();
  numSub = WiFi.subnetMask();
  String stringIp = ipToString(WiFi.localIP());

  Serial.println("Disconnecting...");
  WiFi.disconnect();                           // This doesnt
  while (WiFi.status() == WL_CONNECTED) {
    delay(500);
  }
  Serial.println(WiFi.status() != WL_CONNECTED ? "DISCONNECTED" : "FAILED");

  numIp[0] = stringIp.toInt();

  stringIp.remove(0, 4);
  numIp[1] = stringIp.toInt();

  stringIp.remove(0, 4);
  numIp[2] = stringIp.toInt();


  IPAddress local_IP(numIp[0], numIp[1], numIp[2], 222);
  IPAddress dns(8, 8, 8, 8);

  if (!WiFi.config(local_IP, numGat, numSub, dns))Serial.println("STA Failed to configure");


  wifiManager.autoConnect("EspContaHUB", "contahub");
  Serial.print("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}
