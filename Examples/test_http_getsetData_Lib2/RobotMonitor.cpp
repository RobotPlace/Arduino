#include "RobotMonitor.h"

RobotMonitor::RobotMonitor(const String& authHash, const String& tableinLink, const String& tableoutLink)
  : server(46, 28, 111, 207), clientData(), clientPing(), clientgetData(), authHash(authHash), tableinLink(tableinLink), tableoutLink(tableoutLink), pingperiod(5000), lastping(0), dataperiod(5000), lastdata(0), getdataperiod(5000), lastgetdata(0) {
}

RobotMonitor::~RobotMonitor() {
}

void RobotMonitor::setData(const String& data) {
  if (millis() - lastdata > dataperiod) {
    if (clientData.connect(server, 80)) {
    	dataperiod = 3000;
      // Make a HTTP request:
      // example: http://portal.robotmonitor.com/device/in/3a08b9b0239bc7e979823292f5b6726e/?data={%22millis%22:110465}
      clientData.println("GET /device/in/" + tableinLink + "/?data=" + data + " HTTP/1.0");
      clientData.println("Host: portal.robotmonitor.com");
      clientData.println("Connection: close");
      clientData.println();
      Serial.println("data sent"); //data sent
    }
    else {
      // if you didn't get a connection to the server:
      dataperiod = 10000;
      Serial.println("data failed");
    }
    lastdata = millis();
  }
  
    //waiting server answer
      if (clientData.available()) {
        while (clientData.available()) {
          char c = clientData.read();
          //Serial.print(c);
          if (c == 10) {
        	c = clientData.read();
          if (c == '1') {
              Serial.println(millis() - lastdata);
              Serial.println("setData DONE");
        		clientData.stop();
        		Serial.println(F("disconnected data"));
        		clientData.flush();
            }
          }
        }
      }
}

void RobotMonitor::getData() {
  if (millis() - lastgetdata > getdataperiod) {
    if (clientgetData.connect(server, 80)) {
    	dataperiod = 10000;
      // Make a HTTP request:
      // example: http://portal.robotmonitor.com/device/out/52adc51098c2c424ba55f6ce44a76822
      clientgetData.println("GET /device/out/" + tableoutLink + " HTTP/1.0");
      clientgetData.println("Host: portal.robotmonitor.com");
      clientgetData.println("Connection: close");
      clientgetData.println();
      Serial.println("getdata sent"); //data sent
    }
    else {
      // if you didn't get a connection to the server:
      getdataperiod = 30000;
      Serial.println("getdata failed");
    }
    lastgetdata = millis();
  }
  
    //waiting server answer
      if (clientgetData.available()) {
        while (clientgetData.available()) {
          char c = clientgetData.read();
          Serial.print(c);
          if (c == 13) {
        	c = clientgetData.read();
          if (c == 10) {
          	while (clientgetData.available()){
          		c = clientgetData.read();
          		Serial.print(c);
          	}
              Serial.println(millis() - lastdata);
              Serial.println("getData DONE");
        		clientgetData.stop();
        		Serial.println(F("disconnected getdata"));
        		clientgetData.flush();
            }
          }
        }
      }
}


void RobotMonitor::ping() {
  if (millis() - lastping >= pingperiod) {
    if (clientPing.connect(server, 80)) {
      pingperiod = 3000;
      clientPing.println("GET /device-acces/write/?hash=" + authHash + "&data=pingonline HTTP/1.0");
      clientPing.println("Host: portal.robotmonitor.com");
      clientPing.println("Connection: close");
      clientPing.println();
      Serial.println(" ----------------------------------- ping sent");
    }
    else {
      // if you didn't get a connection to the server:
      pingperiod = 5000;
      Serial.println(F(" ----------------------------------- ping failed"));
    }
    lastping = millis();
   }

  if (clientPing.available()) {
    while (clientPing.available()){
      //Serial.println("available");
      char c = clientPing.read();
       //Serial.print(c);
        if (c == 10){
        	c = clientPing.read();
          if (c == '1'){
            Serial.print(" ----------------------------------- ");
            Serial.println(millis()-lastping);
            Serial.println(" ----------------------------------- PING DONE");
            clientPing.flush();
            clientPing.stop();
            Serial.println(F(" ----------------------------------- disconnected ping"));
          }
        }
    }
  }
}
