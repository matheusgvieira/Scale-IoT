void measurement() {

  // Iniciando Display
  lcd.setBacklight(HIGH);   // Ligando backlight
  lcd.setCursor(0, 0);      // Configurando onde será inserido dados no display
  lcd.print("ContaHUB");    // Inicializando nome da empresa

  medida = 0;
  delay(500);
  medida = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
  Serial.println(medida, 3); // ENVIANDO PARA MONITOR SERIAL A MEDIDA COM 3 CASAS DECIMAIS
  medida += 0.185;



  if (medida < 0.193) {
    medida = 0;
    aux == 0;

    // Send to serial and hardware
    Serial.println(medida, 3);      // Print monitor serial
    dtostrf(medida, 3, 3, str);     // Int to String
    lcd.setCursor(0, 1);            // Position in the display
    lcd.print(str);                 // Print in the display
    mySerial.write(str);            // Enviando valor via serial TX/RX

    while ((medida == 0) && (aux == 0)) {
      medida = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
      aux = (medida < 0.191) ? 0 : 1;
    }
  }

  if (medida > 0.193) {

    // Send to serial and hardware
    medida = medida - 0.185;
    Serial.println(medida, 3);      // Print monitor serial
    dtostrf(medida, 3, 3, str);     // Int to String
    lcd.setCursor(0, 1);            // Position in the display
    lcd.print(str);                 // Print in the display
    mySerial.write(str);            // Sendo value to TX/RX
    aux = medida;

    while ( (medida == aux) || (((aux - 0.06) < medida) && (medida < (aux + 0.06))) ) {
      aux = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
    }
  }
  scale.power_down(); // DESLIGANDO O SENSOR
  delay(1000); // AGUARDA 5 SEGUNDOS
  scale.power_up(); // LIGANDO O SENSOR
}
