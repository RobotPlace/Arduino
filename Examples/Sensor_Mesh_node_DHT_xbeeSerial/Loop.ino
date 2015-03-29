void loop()
{
  /** SoftwareSerial Only **
  if (mySerial.available()){
    //char c = mySerial.read();
    Serial.write(mySerial.read());
  }
  if (Serial.available()){
    //char c = Serial.read();
    mySerial.write(Serial.read());
  }** SoftwareSerial Only **/
  
  //read and resend sensor values
  dataProcess();
}

