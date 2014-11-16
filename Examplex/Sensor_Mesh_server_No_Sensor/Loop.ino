void loop()
{
  //mySerial or Serial listen data packets from other sensors
  //connected by XBee digimesh
  //datas MUST be in the format to be sent "{%22Temp_RH%22:[" id + "," + String(t) + "," + String(h) + "]}";
  //Sensor ID + Temperature value + Humidity value
  if (mySerial.available()>0){
    char c = mySerial.read();
    if(c == '{') dataStart = true;
    if(dataStart) {
      bufferSWSerial += c;
      //Serial.write(c);
    }
    if(c == '}'){
      processRecievedData(bufferSWSerial);
      bufferSWSerial = "";
      dataStart = false;
    }
  }
  if (Serial.available()>0){
    char c = Serial.read();
    bufferSerial += c;
    mySerial.write(c);
    if(c == '\n' || c == '}'){
      bufferSerial = "";
    }
  }
  // HTTP PING ONLINE STATUS TO ROBOTMONITOR SERVER
  // HTTP ANSWER ROBOTMONITOR SERVER URL BY HTTP
  dataProcess();
  pingProcess();
  if( retry == true && clientDATAConection == false ) processRecievedData(retryStr);
}

void processRecievedData(String _str) {
  Serial.println(_str);
  if(_str.substring(0,6).equals("{%22id")){ // ?= "{%22id"
    Serial.print("sDT.");
    if(clientDATAConection == false){
      sendDataTable(_str);
      retry = false;
    } else {
      retry = true;
      retryStr = _str;
      Serial.print("retry data: ");
      Serial.println(_str);
    }
  } 
  else {
    Serial.println("stf");
    //ignored
  }
}














