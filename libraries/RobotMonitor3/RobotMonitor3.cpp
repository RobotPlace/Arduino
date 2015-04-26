#include "RobotMonitor3.h"

RobotMonitor::RobotMonitor(byte *devicemac, const IPAddress& deviceIP)
  : server(46, 28, 111, 207),
    deviceIP(deviceIP), 
    clientPing(), 
    clientData(),
    clientgetData(),
    pingperiod(5000), 
    dataperiod(5000), 
    getdataperiod(5000), 
    lastping(0), 
    lastdata(0), 
    lastgetdata(0)
{
  memcpy(&this->devicemac, devicemac, 6);
}

RobotMonitor::~RobotMonitor() {
}

int RobotMonitor::setData(const String& tableinLink, const String& data) {
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
      return 1;
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
  return 0;
}

int RobotMonitor::getData(const String& tableoutLink, String& data) {
  if (millis() - lastgetdata > getdataperiod) {
    if (clientgetData.connect(server, 80)) {
    	data = "";
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
      return 1;
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
	  c = clientgetData.read();
	  if (c == 13) {
	    c = clientgetData.read();
	    if (c == 10) {
	      c = clientgetData.read();
	      data += c;
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
  }
  return 0;
}

int RobotMonitor::ping(const String& authHash) {
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
      return 1;
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
  return 0;
}

void RobotMonitor::setIP(IPAddress& deviceIp){
	// SET THE LAST NUMBER OF IP ADDRESS
	// IP[3] - EEPROM byte 13, byte 14 for '#'
	// EEPROM.write(14, random(100,255)); //EEPROM.write(14, TrueRandom.randomByte());
	if (EEPROM.read(14) == '#') {
		deviceIP[3] = EEPROM.read(13);
		for (int i = 0; i < 4; i++) {
			Serial.print(deviceIP[i],DEC);
			if(i<4) Serial.print(".");
		}
	} else {
		deviceIP[3] = random(100, 255);//TrueRandom.randomByte();
		EEPROM.write(13, deviceIP[3]);
		EEPROM.write(14, '#');
	}
}

void RobotMonitor::setMAC(uint8_t *devicemac){
  // MAC
  // EEPROM.write(7, random(100,255)); //EEPROM.write(14, TrueRandom.randomByte());
  if (EEPROM.read(7) == '#') {
    for (int i = 0; i < 6; i++) {
      devicemac[i] = EEPROM.read(i);
      Serial.print(devicemac[i],HEX);
      Serial.print(".");
    }
  } else {
    for (int i = 0; i < 6; i++) {
      devicemac[i] = random(100, 255); //TrueRandom.randomByte();
      EEPROM.write(i, devicemac[i]);
    }
    EEPROM.write(7, '#');
  }

}

