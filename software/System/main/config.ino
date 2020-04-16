// Check if photo capture was successful
bool checkPhoto( fs::FS &fs ) {
  File f_pic = fs.open( FILE_PHOTO );
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );
}

// Capture Photo and Save it to SPIFFS
void capturePhotoSaveSpiffs( void ) {
  camera_fb_t * fb = NULL; // pointer
  bool ok = 0; // Boolean indicating if the picture has been taken correctly

  do {
    // Take a photo with the camera
    WebSerial.println("Taking a photo...");

    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    // Photo file name
    Serial.printf("Picture file name: %s\n", FILE_PHOTO);
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);

    // Insert the data in the photo file
    if (!file) {
      WebSerial.println("Failed to open file in writing mode");
    }
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      WebSerial.print("The picture has been saved in ");
      WebSerial.print(FILE_PHOTO);
      WebSerial.print(" - Size: ");
      WebSerial.print(file.size());
      WebSerial.println(" bytes");
    }
    // Close the file
    file.close();
    esp_camera_fb_return(fb);

    // check if file has been correctly saved in SPIFFS
    ok = checkPhoto(SPIFFS);
  } while ( !ok );
}

void recvMsg(uint8_t *data, size_t len) {
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);
}

String czero(String json) {
  int len = json.length() + 1;
  char aux[len];
  json.toCharArray(aux, len);

  if (aux[11] == '0' and aux[10] == ' ') {
    for (int i = 11; i < 16; i++) {
      aux[i] = aux[i + 1];
    }
  }
  return aux;
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

  if (!WiFi.config(local_IP, numGat, numSub))Serial.println("STA Failed to configure");


  wifiManager.autoConnect("EspContaHUB", "contahub");
  Serial.print("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}
