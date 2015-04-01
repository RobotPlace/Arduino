#include "RobotMonitor.h"

RobotMonitor::RobotMonitor(const String& authHash)
  : server(46, 28, 111, 207), client(), authHash(authHash), pingperiod(5000), dataperiod(5000), lastping(0), lastdata(0), clientConection(false), clientAnswered(false) {
  tableinLink = "3a08b9b0239bc7e979823292f5b6726e";
}

RobotMonitor::~RobotMonitor() {
}

boolean RobotMonitor::setData(const String& data) {
  if (millis() - lastdata > dataperiod) {
    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
      // Make a HTTP request:
      //portal.robotmonitor.com/device/in/3a08b9b0239bc7e979823292f5b6726e/?data={%22millis%22:110465}
      Serial.print(data);
      clientConection = true;
      client.println("GET /device/in/" + tableinLink + "/?data=" + data + " HTTP/1.0");
      client.println("Host: portal.robotmonitor.com");
      client.println("Connection: close");
      client.println();
      Serial.println("ds!"); //data sent
    }
    else {
      // if you didn't get a connection to the server:
      clientConection = false;
      dataperiod = 10000;
      Serial.println("data failed");
    }
    lastdata = millis();

    //waiting server answer
    if (clientConection) {
      if (client.available()) {
        while (client.available()) {
          char c = client.read();
          //Serial.print(c);
          if (c == 10) {
            c = client.read();
            if (c == '1') {
              Serial.println(c);
              Serial.println(millis() - lastdata);
              Serial.println("setData DONE");
              clientAnswered = true;
              client.flush();
            }
          }
        }
      }
      
      // if the server is disconnected, stop the client:
      if (clientConection == true && clientAnswered == true) {
        client.stop();
        clientConection = false;
        clientAnswered = false;
        Serial.println(F("disconnected"));
      }
    }
    return true;
  }
  return false;
}

void RobotMonitor::ping() {
  // if you get a connection, report back via serial:
  if (millis() - lastping >= pingperiod) {
    if (client.connect(server, 80)) {
      clientConection = true;
      clientAnswered = false;
      pingperiod = 60000;
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
      pingperiod = 600000;
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
          }
        }
    }
  }
  
  // if the server's disconnected, stop the client:
  if (clientConection == true && clientAnswered == true) { //!client.connected() && 
    client.stop();
    clientConection = false;
    Serial.println(F("disconnected"));
  }
}
