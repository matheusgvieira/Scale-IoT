
/*
    Autor: Matheus Gois Vieira
    Empresa: ContaHub
    Sketch:  Módulo para Arduino HX711
    Data: 20/02/2020
*/

// INCLUSÃO DE BIBLIOTECAS
#include <HX711.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DEFINIÇÕES DE PINOS
#define pinDT  2
#define pinSCK  3

// INSTANCIANDO OBJETOS
HX711 scale;
SoftwareSerial mySerial(10, 11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DECLARAÇÃO DE VARIÁVEIS
float medida = 0;
char str[80];
int aux = 0;

void setup() {
  Serial.begin(57600);
  mySerial.begin(115200);
  lcd.begin(16, 2);
  lcd.init();


  scale.begin(pinDT, pinSCK); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.set_scale(20702); // LIMPANDO O VALOR DA ESCALA

  delay(2000);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA
  Serial.println("Iniciando Balança");

  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("ContaHUB");
}

void loop() {

  medida = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
  Serial.println(medida, 3); // ENVIANDO PARA MONITOR SERIAL A MEDIDA COM 3 CASAS DECIMAIS

  

  if (medida < 0.06) {
    medida = 0;
    aux == 0;

    // Send to serial and hardware
    Serial.println(medida, 3);      // Print monitor serial
    dtostrf(medida, 3, 3, str);     // Int to String
    lcd.setCursor(0, 1);            // Position in the display
    lcd.print(str);                 // Print in the display
    mySerial.write(str);            // Enviando valor via serial TX/RX

    while ((medida == 0) && (aux == 0)) {
      medida = scale.get_units(10); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 10 MEDIDAS
      aux = (medida < 0.06) ? 0 : 1;
    }
  }

  else {

    // Send to serial and hardware
    Serial.println(medida, 3);      // Print monitor serial
    dtostrf(medida, 3, 3, str);     // Int to String
    lcd.setCursor(0, 1);            // Position in the display
    lcd.print(str);                 // Print in the display
    mySerial.write(str);            // Sendo value to TX/RX
    aux = medida;

    while ( (medida == aux) || (((aux - 0.06) < medida) && (medida < (aux + 0.06))) ) {
      aux = scale.get_units(10); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 10 MEDIDAS
    }
  }
  scale.power_down(); // DESLIGANDO O SENSOR
  delay(1000); // AGUARDA 5 SEGUNDOS
  scale.power_up(); // LIGANDO O SENSOR
}
