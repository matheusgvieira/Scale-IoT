#define led 13
char vector[] = {0, 0, 0, 0, 0, 0, 0};
int value, a = 0;
int n = 7;
float resultado = 0;

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
    Serial.println("--------------------");
    Serial.println("Peso:");
    if (vector[n - 1] == 'k') {
      for (int i = 0; i < n; i++) Serial.print(vector[i]);
      Serial.println("g");
    } else {
      Serial.println("Lendo ...");
    }
    Serial.println("--------------------");

  }
  delay(1000);
}
