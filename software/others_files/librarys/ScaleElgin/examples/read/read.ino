/*
    Autor: Matheus Gois Vieira
    Março de 2020
    Obs: Utilize um conversor RS232-TTL, conecte:

    Arduino     Conversor
    Rx      ->  Rx
    Tx      ->  Tx

*/

#include <Arduino.h>
#include <ScaleElgin.h>

String peso;
ScaleElgin scale(peso);

void setup(){
    Serial.begin(9600);
}
void loop(){
    String weight = scale.readScale();
    Serial.print("Peso:");
    Serial.println(weight);
}