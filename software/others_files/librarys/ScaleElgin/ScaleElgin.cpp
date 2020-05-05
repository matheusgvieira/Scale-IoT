/*
    Autor: Matheus Gois Vieira
    Março de 2020
    Obs: Utilize um conversor RS232-TTL, conecte:

*/

#include "ScaleElgin.h"
#include "string.h"
#include "Arduino.h"
#include "ArduinoJson.h"

char  vector[] = {0, 0, 0, 0, 0, 0, 0},
      peso_aux[] = {0, 0, 0, 0, 0, 0}, 
      letra;
int value,
    n = 7,
    ct = 0,
    s = 0;
String w;
bool casoZero = false;
  

ScaleElgin::ScaleElgin(){
}

String czero(String json) {
  int len = json.length() + 1;
  char aux[len];
  json.toCharArray(aux, len);

  if (aux[1] == '0' and aux[0] == ' ') {
    for (int i = 1; i < len; i++) {
      aux[i] = aux[i + 1];
    }
  }
  return aux;
}

// char * stringToChar(String vString){
//   int len = vString.length() + 1;
//   char vChar[len];
//   vString.toCharArray(vChar, len);
//   return vChar;
// }

String ScaleElgin::readScale() {
  
  while (1) {
    // Send "5" for scale
    Serial.write(5);

    // Stop, case don't finds nothing
    ct++;
    if(ct == 5) {
      ct = 0;
      casoZero = true;
      break;
    }
    //If there is an answer do...
    if (Serial.available() > 0) {   

      // lê o dado recebido:
      for (int c = 0; c < n; c++) {

        value = Serial.read();

        switch (value) {
          case -1:
            vector[c] = '.';
            break;
          case 2:
            break;
          case 3:
            vector[c] = 'k';
            break;
          default:
            vector[c] = value;
        }

      }

      if (vector[n - 1] == 'k') {
        for (int i = 0; i < 7; i++) {
          if (i >= 1) peso_aux[i - 1] = vector[i];
        }
        break; // when you find the correct value, stop the repetition
      }
    }
    delay(100);
  }

  // Return peso
  weight = String(peso_aux);
  weight.remove(weight.length()-1, 1); // Remove "k"

  if(casoZero == true){
    weight.remove(0);
    casoZero = false;
  }  
  
  //String  To float
  float kilos = weight.toFloat();

  StaticJsonDocument<800> doc;
  doc.clear();
  doc["resultado"] = true;
  doc["buffer"] = "";
  doc["gramas"] = czero(weight).toInt();
  doc["precoKg"] = kilos/1000;
  doc["total"] = "";
  doc["timestamp"] = millis();
  w.remove(0);
  serializeJsonPretty(doc, w); 

  //w = "{\"gramas\":" + weight + "}";

  return w;
}