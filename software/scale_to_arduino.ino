#define led 13
char vector[] = {0,0,0,0};
char value;

void setup() {
  // Configure the multiplexer select pin
  Serial.begin(9600);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

}

void loop() {
  Serial.write(5);
  // enviar resposta apenas quando receber dados:
  if (Serial.available() > 0) {
    // lê o dado recebido:
    for(int c = 0; c < 5; c++){
      value = Serial.read();
      if(value == null){
        c = 0;
      }
      else{
        vector[c] = value;
      }      
    }
    Serial.println("Valor:");
    Serial.print(vector);
    Serial.println("kg");
    Serial.println("--------------------");
    
  }
  delay(1000);
}
