void writeString(char add, String content)
{
  int _size = content.length();
  int i;
  for (i = 0; i < _size; i++)
  {
    EEPROM.write(add + i, content[i]);
  }
  EEPROM.write(add + _size, '\0'); //Adicionar caractere nulo de terminação para dados da string
  EEPROM.commit();
}


String read_String(char add)
{
  int i;
  char content[100]; //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while (k != '\0' && len < 500) //Ler até caractere nulo
  {
    k = EEPROM.read(add + len);
    content[len] = k;
    len++;
  }
  content[len] = '\0';
  return String(content);
}

void reading() {
  
  while (1) {
    // Send "5" for scale
    Serial.write(5);

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
          if (i >= 1) peso[i - 1] = vector[i];
        }
        Serial.println();
        break; // when you find the correct value, stop the repetition
      }
    }
    delay(100);
  }
  // Send for memory flash
  Serial.print("Peso:");
  tpeso = String(peso);
  Serial.println(tpeso + 'g');
  writeString(10, tpeso);

  delay(10);
}
