int calibration() {
  int b = 0;
  float medida = 0, med = 0;


  scale.set_scale(); // LIMPANDO O VALOR DA ESCALA

  while (b == 0) {
    lcd.setBacklight(HIGH);   // Ligando backlight
    lcd.setCursor(0, 0);      // Configurando onde será inserido dados no display
    lcd.print("Calibration");    // Inicializando nome da empresa

    lcd.setCursor(0, 1);      // Configurando onde será inserido dados no display
    lcd.print("Coloque 0.185kg");    // Inicializando nome da empresa

    delay(10000);

    for (int cont = 0; cont < 100; cont++) {
      medida = scale.get_units(10); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 10 MEDIDA
      scale.power_down(); // DESLIGANDO O SENSOR
      delay(500); // AGUARDA 0.5 SEGUNDO
      scale.power_up(); // LIGANDO O SENSOR
      med += medida;
    }

    c = (med/100) / 0.185;
  }
  return c;
}
