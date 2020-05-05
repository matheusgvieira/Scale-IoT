#include "RegisterEsp.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>


//Creat json
StaticJsonDocument<800> doc;

//Create Variables

//char mac[] = "24:6F:28:15:71:4C",
char* mac, aux = 'a';
String tipo = "01",
       payload,
       val1,
       val2,
       val3,
       x,
       y;
unsigned long Time;

boolean condition;

int j = 0,
    equal = 0;

///////////////////////////// Metodo Construtor ////////////////////////////////////////
RegisterEsp::RegisterEsp(char macAdress[], String url)
{
  _url = url;
  mac = macAdress;
}
///////////////////////////// Métdos Especiais ////////////////////////////////////////
String parsingJson(String json)
{
  StaticJsonDocument<200> root;
  DeserializationError error = deserializeJson(root, json);

  if (error)
  { //Check for errors in parsing

    Serial.println("Parsing failed");
    condition = true;
    delay(5000);
  }
  const char *value = root["register"]; //Get sensor type value
  while (aux != '\0')
  {
    aux = value[j];
    if ((y[j] == '=') && (equal == 2))
    {
      val3 = String(y);
      val3.remove(j + 1);
      val3 = val3 + String(Time);
      break;
    }
    if ((x[j] == '=') && (equal == 1))
    {
      val2 = String(x);
      val2.remove(j + 1);
      val2 = val2 + tipo + '&';
      x.remove(0, j + 8);
      y = x;
      j = 0;
      equal++;
    } 
    if ((value[j] == '=') && (equal == 0))
    {
      val1 = String(value);
      val1.remove(j + 1);
      val1 = val1 + mac + '&';
      x = String(value);
      x.remove(0, j + 7);
      j = 0;
      equal++;
    }
    j++;
  }
  json = val1 + val2 + val3;
  Serial.println(json);
  condition = false;
  return json;
}
/////////////////////////////////////////////////////////////////////////////////////////
String postJson(String url)
{
  char jsonOutput[128];
  if ((WiFi.status() == WL_CONNECTED))
  {
    HTTPClient client;

    client.begin(url);
    client.addHeader("Content-Type", "application/json");

    const size_t CAPACITY = JSON_OBJECT_SIZE(1);
    StaticJsonDocument<CAPACITY> doc;

    JsonObject object = doc.to<JsonObject>();
    object["title"] = "Iniciando uma EspContaHUB";

    serializeJson(doc, jsonOutput);

    int httpCode = client.POST(String(jsonOutput));

    if (httpCode > 0)
    {
      //Check for the returning code
      String payload = client.getString();
      //Serial.println("\nStatuscode:" + String(httpCode));
      //Serial.println(payload);
      condition = false;
      return payload;

      client.end(); //Free the resources
    }
    else
    {
      Serial.println("Error on HTTP request");
      condition = true;
    }
  }
  else
  {
    Serial.println("Connection lost");
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
String getJson(String url)
{
  if ((WiFi.status() == WL_CONNECTED))
  { //Check the current connection status

    HTTPClient http;

    http.begin(url);           //Specify the URL
    int httpCode = http.GET(); //Make the request

    if (httpCode > 0)
    { //Check for the returning code

      String payload = http.getString();
      //Serial.println(httpCode);
      //Serial.println(payload);
      condition = false;
      return payload;
    }

    else
    {
      Serial.println("Error on HTTP request");
      condition = true;
    }

    http.end(); //Free the resources
  }
}
void printJson(String Json)
{
  DeserializationError error = deserializeJson(doc, Json);
  Json.remove(0);
  serializeJsonPretty(doc, Json);
  Serial.println(Json);
}
String toChar(String json) {
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

void RegisterEsp::begin()
{
  String json = getJson(_url);
  String parameterInit = parsingJson(json);
  String parameterEsp = postJson(parameterInit);
  deserializeJson(doc, parameterEsp);
  //printJson(parameterEsp);
}

int RegisterEsp::dev()
{
  int value = doc["dev"];
  return value;
}

int RegisterEsp::emp()
{
  int value = doc["emp"];
  return value;
}

String RegisterEsp::dev_mac()
{
  String value = doc["dev_mac"];
  return value;
}
String RegisterEsp::dev_cfg()
{
  String value = doc["dev_cfg"];
  return value;
}
String RegisterEsp::dev_desc()
{
  String value = doc["dev_desc"];
  return value;
}
String RegisterEsp::dev_hrultima()
{
  String value = doc["dev_hrultima"];
  return value;
}
String RegisterEsp::nowDate()
{
  String value = doc["_now"];
  // Extract date
  int splitT = value.indexOf("T");
  String day = value.substring(0, splitT);  
  return day;
}
int RegisterEsp::nowTimeSegundos()
{
  String value = doc["_now"];
  // Extract time
  int splitT = value.indexOf("T");
  String Time = value.substring(splitT+1, value.length()-1);

  String clock = toChar(Time);

  String hours = String(clock[0]) + String(clock[1]);
  String min = String(clock[3]) + String(clock[4]);
  String seg = String(clock[6]) + String(clock[7]);
  
  int intHours = hours.toInt();
  int intMin = min.toInt();
  int intSeg = seg.toInt();

  int timeSegundos = (hours.toInt())*3600 + (min.toInt())*60 + seg.toInt();

  //Time = Time + " / " + String(timeSegundos);

  return timeSegundos;
}

String RegisterEsp::nowTimeJson()
{
  String value = doc["_now"];
  // Extract time
  int splitT = value.indexOf("T");
  String Time = value.substring(splitT+1, value.length()-1);

  return Time;
}
String RegisterEsp::token()
{
  String value = doc["_token"];
  return value;
}
long int RegisterEsp::timestamp()
{
  long int value = doc["_timestamp"];
  return value;
}