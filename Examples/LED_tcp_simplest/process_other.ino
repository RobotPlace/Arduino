void processData(String _str) {

  //parse the message to commands
  int switch_value = -1;
  if (_str[0] == 'S' || _str[0] == 's') switch_value = 0;
  else if (_str[0] == 'A' || _str[0] == 'a') switch_value = 1;
  else if (_str[0] >= 48 && _str[0] <= 57) switch_value = 2; //we have got number of users online
  else if (_str[0] == 'E') switch_value = 3; //E (missing parameter)
  if (switch_value < 3) permanentError = 0;

  switch (switch_value) {
    case 0:
      Serial.println(F("=>S")); // LED ON
      ledState = 0;
      ledBlink(ledState);
      break;
    case 1:
      Serial.println(F("=>A")); // LED OFF
      ledState = 1;
      ledBlink(ledState);
      break;
    case 2:
      Serial.println(F("=> users online"));
      logged = true;
      break;
    case 3:
      Serial.println(F("=> Login error"));
      permanentError++;
      Serial.print(F("=> permanentError = "));
      Serial.println(permanentError);
      delay(1000);
      sendTCP("1");
      if ( permanentError > 5 ) {
        Serial.println(F("permanentError > 5 => 1 minute ON HOLD"));
        permanentError = 0;
        delay(60000);
      }
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      delay(10);
  }
  delay(10);
}

void ledBlink(int _switch) {
  if (_switch == 1) analogWrite(ledPin, 255); //PWM transforms 5V (255) to 3V (153)
  if (_switch == 0) analogWrite(ledPin, 0);
  lastLED = millis();
}

void led_reset() {
  if ( (millis() - lastLED > resetLEDtime) && ledState != 0 ) {
    ledState = 0;
    ledState = 0;
    ledBlink(ledState);
  }
}

