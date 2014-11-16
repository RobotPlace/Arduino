void pingProcess()
{
  // When HTTP request answer bytes available 
  // from the server, read them and print them:
  while (clientPING.available()) {
    char c = clientPING.read();
    bufferPING += c;
    //Buffer can't contain the whole http content
    if(bufferPING.length() > 80){
      bufferPING = bufferPING.substring((bufferPING.length()-25),(bufferPING.length()));
    }
  }

  // if the server's disconnected, stop the client:
  if (!clientPING.connected() && clientPINGConection == true) {
    clientPING.stop();
    clientPINGConection = false;
    parsePingAnswer();
  }

  if(millis() - lastping >= pingperiod){
    ping();
    lastping = millis();
    //if ping fails in 60ms, repeat next ping in 3s
    if(pingDone == false) lastping = millis() - pingperiod + 30000;
  }
}

void ping()
{
  // if you get a connection, report back via serial:
  if (clientPING.connect(server, 80)) {
    clientPINGConection = true;
    pingDone = true;
    clientPING.println("GET /device-acces/write/?hash=" + KEY + "&data=ping HTTP/1.0");
    clientPING.println("Host: portal.robotmonitor.com");
    clientPING.println("Connection: close");
    clientPING.println();
    Serial.println("Ps");
  }
  else {
    // if you didn't get a connection to the server:
    pingDone = false;
    PGerrors++;
    Serial.println("Pf");
    Serial.print("Ping errors: ");
    Serial.println(PGerrors);
  }
}

void parsePingAnswer(){
  //cut only the server name "xxx.robotmonitor.com"
  bufferPING = bufferPING.substring((bufferPING.length()-25),(bufferPING.length()-5));
  for(int i=0; i<=3;i++){
    //liveDataServerName[i] = bufferHTTP[i];
  }
  //liveDataServerGOT = true;
  bufferPING = "";
}




