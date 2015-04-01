//NO CHANGE NEEDED FROM USER
void sendTCP(String _str)
{
  Serial.print(".");
  if (client.connected()){
    client.print(_str);
  }  // if the server's disconnected, stop the client:
  else if (!client.connected()){
      Serial.println(F("disconnected"));
      client.stop();
      connectTCP();
    }
  lastConnected = millis();
}

//NO CHANGE NEEDED FROM USER
void connectTCP()
{
  Serial.print(",");
  // if you get a connection, report back via serial:
  if (client.connect(serverTCP, 8080)) {
    Serial.println(F("reConnectTCP():"));
    Serial.println(loginStr);
    client.println(loginStr);
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println(F("connection failed"));
  }
}

void readTCP(){
  // if there are incoming bytes available 
  // from the server, read them and print them to robots:
  if(client.available()){
    char c = client.read();
    bufferTCP += c;
    //Serial.print(c);
    //check if there are listeners
    if(bufferTCP.length() == 1){
      if (c >= 48 || c <= 57){
        listeners = int(c) - 48;
        if(listeners > 0 && listeners <= 9){
          Serial.println(F("listeners > 0"));
          connRefreshPeriod = 5000;
        }
        else if(listeners = 0){
          Serial.println(F("listeners = 0"));
          connRefreshPeriod = 2000;
        }
      }
    }
    bufferTCPcomplete = true;
    lastConnected = millis();
  }
  else {
    if(bufferTCPcomplete){
      //Serial.print(bufferTCP);
      processData(bufferTCP);
      bufferTCP = "";
      bufferTCPcomplete = false;
    }
  }
}

//NO CHANGE NEEDED FROM USER
//heartbeat() is optional. It solves server disconnection if no data are sent for more than 10 minutes
void connRefresh(){
  if(millis()-lastConnected > connRefreshPeriod){
    Serial.println(F("REFRESH"));
    //send or reconnect
    sendTCP("@");
    sendTCP((String)lastConnected);
    sendTCP("\r\n");
    Serial.print("@");
    Serial.print((String)lastConnected);
    Serial.println();
  }
}

