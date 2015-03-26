void loop()
{
  /* I dont need read XBee incomming data
  if (mySerial.available()){
    //char c = mySerial.read();
    Serial.write(mySerial.read());
  }
  */
  if (Serial.available()){
    //char c = Serial.read();
    mySerial.write(Serial.read());
  }
  //read and resend sensor values
  dataProcess();
}

