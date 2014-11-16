//    String datas = linkStart  linkSeparator  linkEnd

void dataProcess()
{

    // When HTTP request answer bytes available 
    // from the server, read them and print them:
    while (clientDATA.available()) {
      char c = clientDATA.read();
      bufferData += c;
      //Serial.print(c);
      //Buffer can't contain the whole http content
      if(bufferData.length() > 200){
        bufferData = bufferData.substring((bufferData.length()-25),(bufferData.length()));
      }
    }
    // if the server's disconnected, stop the client:
    if (!clientDATA.connected()  && clientDATAConection == true ) {
      clientDATA.stop();
      clientDATAConection = false;
      //Serial.println("clientDATA disconnected");
      //Serial.print("parseDataAnswer(): ");
      parseDataAnswer();
    }
    
  if(onboardSensor){
    if(millis()-lastdataRead > dataReadPeriod){
      readData();
      if(t != tLast || h != hLast || (millis()-lastdataSend > dataSendPeriod)){
        String datas = "{%22id%22:%22" + XBeeID + "%22,%22Temp_RH%22:[" + String(t) + "," + String(h) + "]}";
        sendDataTable(datas);
        lastdataSend = millis();
      }
      else {
        Serial.println("nc");
      }
      lastdataRead = millis();
    }
  }
}

void readData()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  hLast = h;
  tLast = t;
  h = dht.readHumidity();
  t = dht.readTemperature();
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("FD");
  }
  else {
    //Serial.print(t);
    //Serial.print("C ");
    //Serial.println(h);
  }
}

void sendDataTable(String _str)
{
  // if you get a connection, report back via serial:
  if (clientDATA.connect(server, 80)) {
    //Serial.println("connected");
    clientDATAConection = true;
    // Make a HTTP request:
    clientDATA.println("GET /device/in/"+LINK+"/?data="+_str+" HTTP/1.0");
    clientDATA.println("Host: portal.robotmonitor.com");
    clientDATA.println("Connection: close");
    clientDATA.println();
    Serial.println("ds!");
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("cf");
  }
}

void parseDataAnswer(){
  String answer = bufferData.substring((bufferData.length()-1),(bufferData.length()));
  if(answer != "1") {
    Serial.println(bufferData); //dataTable writing failed
    DTerrors++;
    Serial.print("DataTable errors: ");
    Serial.println(DTerrors);
  }
}






