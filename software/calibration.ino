void calibration() {
  float medida = 0, med = 0;


  scale.set_scale(); // LIMPANDO O VALOR DA ESCALA

  while (c == 17248) {
    lcd.setBacklight(HIGH);   // Ligando backlight
    lcd.setCursor(0, 0);      // Configurando onde será inserido dados no display
    lcd.print("Calibration");    // Inicializando nome da empresa

    lcd.setCursor(0, 1);      // Configurando onde será inserido dados no display
    lcd.print("Put 0.185kg");    // Inicializando nome da empresa

    buttonWait(b2Pin);

    for (int cont = 0; cont < 50; cont++) {
      medida = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDA
      scale.power_down(); // DESLIGANDO O SENSOR
      delay(30); // AGUARDA 0.5 SEGUNDO
      scale.power_up(); // LIGANDO O SENSOR
      med += medida;
      lcd.setCursor(0, 1);      // Configurando onde será inserido dados no display
      lcd.print("...             ");    // Inicializando nome da empresa
    }
    

    c = (med / 50) / 0.185;
    Serial.println(c);

    setup_scale();

    lcd.clear();
    lcd.setCursor(0, 0);      // Configurando onde será inserido dados no display
    lcd.print("Calibrated");    // Inicializando nome da empresa
    lcd.setCursor(0, 1);      // Configurando onde será inserido dados no display
    lcd.print("scale!");    // Inicializando nome da empresa
    delay(2000);
    lcd.clear();
  }
  return;
}

void buttonWait(int buttonPin) {
  while (1) {
    if (!digitalRead(b2Pin)) tb2 = 0x01;
    if (digitalRead(b2Pin) && tb2) {
      return;
    }
  }
}
