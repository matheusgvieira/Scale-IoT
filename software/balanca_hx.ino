
/*
    Autor: Matheus Gois Vieira
    Empresa: ContaHub
    Sketch:  Protótipo de Balança
    Data: 26/02/2020
*/

/////////////////////////////////////////////////////////////////////////////////////

// INCLUSÃO DE BIBLIOTECAS
#include <HX711.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/////////////////////////////////////////////////////////////////////////////////////

// DEFINIÇÕES DE PINOS
#define pinDT  4
#define pinSCK  5
#define b1Pin 2
#define b2Pin 3

/////////////////////////////////////////////////////////////////////////////////////

// INSTANCIANDO OBJETOS
HX711 scale;
SoftwareSerial mySerial(10, 11);
LiquidCrystal_I2C lcd(0x27, 16, 2);


/////////////////////////////////////////////////////////////////////////////////////

// DECLARAÇÃO DE VARIÁVEIS
float medida = 0, aux = 0;
char str[80];
int c = 0, a = 1;
boolean tb2;
volatile byte state = LOW;
const byte ledPin = 13;

/////////////////////////////////////////////////////////////////////////////////////

void setup() {

  // Configurando a velocidade da cominicação serial
  Serial.begin(57600);  // Configurando velocidade do monitor serial
  mySerial.begin(115200);   // Configurando velocidade do envio via serial

  // Configurando o Display 16x02
  lcd.begin(16, 2); // Tamanho do display
  lcd.init();   // Inicializando o display 16x02

  // Habilitando pinos do microcontrolador
  pinMode(ledPin, OUTPUT);
  pinMode(b1Pin, INPUT_PULLUP);
  pinMode(b2Pin, INPUT_PULLUP);

  // Habilitando e configurando a interrupção no pino 2
  attachInterrupt(digitalPinToInterrupt(b1Pin), changeDisplay1, LOW);
  attachInterrupt(digitalPinToInterrupt(b2Pin), changeDisplay2, LOW);

  // Configurando hx711
  scale.begin(pinDT, pinSCK); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.set_scale(c); // LIMPANDO O VALOR DA ESCALA
  delay(2000);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA
  Serial.println("Iniciando Balança");

  // Iniciando Display
  lcd.setBacklight(HIGH);   // Ligando backlight
  lcd.setCursor(0, 0);      // Configurando onde será inserido dados no display
  lcd.print("ContaHUB");    // Inicializando nome da empresa
}
void loop() {
  if (a == 0) {
    calibration();
  }
  if (a == 1) {
    measurement();
  }
}
void changeDisplay1() {
  static bool estado = LOW;
  static unsigned long delayEstado;


  if ( (millis() - delayEstado) > 100 ) {
    state = !state;
    delayEstado = millis();
  }
  a = 0;
}
void changeDisplay2() {
  static bool estado = LOW;
  static unsigned long delayEstado;


  if ( (millis() - delayEstado) > 100 ) {
    state = !state;
    delayEstado = millis();
  }
  a = 1;
}

/////////////////////////////////////////////////////////////////////////////////////
