
#include "RobotMonitor.h"

RobotMonitor::RobotMonitor(const String authHash)
  : server(46, 28, 111, 207), client(), authHash(authHash), pingperiod(5000), lastping(0), clientConection(false), clientAnswered(false) {
}

RobotMonitor::~RobotMonitor() {
}

void RobotMonitor::ping() {
  // if you get a connection, report back via serial:
  if (millis() - lastping >= pingperiod) {
    if (client.connect(server, 80)) {
      clientConection = true;
      pingperiod = 10000;
      Serial.println("connected");
      client.println("GET /device-acces/write/?hash=" + authHash + "&data=pingonline HTTP/1.0");
      client.println("Host: portal.robotmonitor.com");
      client.println("Connection: close");
      client.println();
      Serial.println("ping OK");
    }
    else {
      // if you didn't get a connection to the server:
      clientConection = false;
      pingperiod = 30000;
      Serial.println(F("ping failed"));
    }
    lastping = millis();
   }

  if (client.available()) {
    while (client.available()){
      //Serial.println("available");
      char c = client.read();
      //Serial.print(c);
        if (c == 10){
          c = client.read();
          if (c == '1'){
            Serial.println(c);
            Serial.println(millis()-lastping);
            Serial.println("PING DONE");
            clientAnswered = true;
            client.flush();
          }
        }
    }
  }
  
  // if the server is disconnected, stop the client:
  if (clientConection == true && clientAnswered == true) { //!client.connected() && 
    client.stop();
    clientConection = false;
    clientAnswered = false;
    Serial.println(F("disconnected"));
  }
}
