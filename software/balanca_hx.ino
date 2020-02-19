
/*
 *  Autor: Matheus Gois Vieira
 *  Empresa: ContaHub
 *  Sketch:  Módulo para Arduino HX711
 *  Data: 27/01/2020
*/


// INCLUSÃO DE BIBLIOTECAS
#include <HX711.h>
#include <PushButton.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



// DEFINIÇÕES DE PINOS
#define pinDT  2
#define pinSCK  3
#define pinBotao 4

// DEFINIÇÕES
#define pesoMin 0.010
#define pesoMax 30.0

#define escala 0.0f

// INSTANCIANDO OBJETOS
HX711 scale;
PushButton botao(pinBotao);
SoftwareSerial mySerial(10,11);
LiquidCrystal_I2C lcd(0x27,16,2);

// DECLARAÇÃO DE VARIÁVEIS  
float medida=0;
char str[80];

void setup() {
  Serial.begin(57600);
  mySerial.begin(115200);
  lcd.begin(16,2);
  lcd.init();

  scale.begin(pinDT, pinSCK); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.set_scale(100815); // ENVIANDO O VALOR DA ESCALA CALIBRADO

  delay(2000);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA
  Serial.println("Setup Finalizado!");

  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("ContaHUB");
}

void loop() {
  botao.button_loop(); // LEITURA DO BOTAO

  if(botao.pressed()){ // QUANDO PRESSIONAR O BOTÃO
    
    scale.power_up(); // LIGANDO O SENSOR
    
    medida = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
    
    if (medida <= pesoMin ){ // CONFERE SE A MASSA ESTÁ NA FAIXA VÁLIDA
      scale.tare(); // ZERA A BALANÇA CASO A MASSA SEJA MENOR QUE O VALOR MIN
      medida = 0;
      Serial.println("Tara Configurada!");
    } else if ( medida >= pesoMax ){
      scale.tare(); // ZERA A BALANÇA CASO A MASSA SEJA MAIOR QUE O VALOR MÁX
      medida = 0;
      Serial.println("Tara Configurada!");
    } else {
      Serial.println(medida,3);
   
      dtostrf(medida,3,3,str);
      Serial.println("-------------------------");
      Serial.println(str);
      Serial.println("-------------------------");
      lcd.setCursor(0,1);
      lcd.print(str);
      mySerial.write(str);
      Serial.println("Valor enviado");
    }

    scale.power_down(); // DESLIGANDO O SENSOR
  }
  
}

// IMPLEMENTO DE FUNÇÕES
