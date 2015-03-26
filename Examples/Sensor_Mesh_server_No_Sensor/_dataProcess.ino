//    String datas = linkStart  linkSeparator  linkEnd
void dataProcess(){
  //waiting server answer
  if(clientDATAConection){
    if (clientDATA.available()) {
      char c = clientDATA.read();
      bufferData += c;
      //Serial.print(c);
      //Buffer can't contain the whole http content
      if(bufferData.length() > 80){
        bufferData = bufferData.substring((bufferData.length()-25),(bufferData.length()));
      }
    }
    // if the server's disconnected, stop the client:
    if (!clientDATA.connected()  && clientDATAConection == true ) {
      clientDATA.stop();
      clientDATAConection = false;
      parseDataAnswer();
    }
  }
}

void sendDataTable(String _str)
{
  // if you get a connection, report back via serial:
  if (clientDATA.connect(server, 80)) {
    dataVoidTimeLast = millis();
    //Serial.println("connected");
    clientDATAConection = true;
    // Make a HTTP request:
    clientDATA.println("GET /device/in/"+tableinLink+"/?data="+_str+" HTTP/1.0");
    clientDATA.println("Host: portal.robotmonitor.com");
    clientDATA.println("Connection: close");
    clientDATA.println();
    Serial.print("ds!");
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("cf");
  }
}

void parseDataAnswer(){
  String dataAnswer = bufferData.substring((bufferData.length()-1),(bufferData.length()));
  if(dataAnswer != "1") {
    Serial.println(bufferData); //dataTable writing failed
    DTerrors++;
    Serial.print("DataTable errors: ");
    Serial.println(DTerrors);
  } 
  else {
    Serial.print("dsOK");
    dataVoidTime = millis() - dataVoidTimeLast;
    Serial.println(dataVoidTime);
  }
  bufferData = "";
}








