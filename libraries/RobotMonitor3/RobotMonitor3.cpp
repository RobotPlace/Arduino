#include "RobotMonitor3.h"

RobotMonitor::RobotMonitor(byte *devicemac, const IPAddress& deviceIP)
  : server(46, 28, 111, 207),
    deviceIP(deviceIP), 
    clientPing(), 
    clientsetData(),
    clientgetData(),
    pingperiod(30000), 
    setdataperiod(10000), 
    getdataperiod(10000), 
    lastping(0), 
    lastdata(0), 
    lastgetdata(0),
    clientConection(false),
    clientAnswered(false)
{
  memcpy(&this->devicemac, devicemac, 6);
}

RobotMonitor::~RobotMonitor() {
}

void RobotMonitor::setData(const String& tableinLink, String& data) {
  if (millis() - lastdata > setdataperiod) {
    if (clientsetData.connect(server, 80)) {
      setdataperiod = 25000;
      // Make a HTTP request:
      // example: http://portal.robotmonitor.com/device/in/3a08b9b0239bc7e979823292f5b6726e/?data={%22millis%22:110465}
      clientsetData.println("GET /device/in/" + tableinLink + "/?data=" + data + " HTTP/1.0");
      clientsetData.println("Host: portal.robotmonitor.com");
      clientsetData.println("Connection: close");
      clientsetData.println();
      Serial.println("setdata sent"); //data sent
    }
    else {
      // if you didn't get a connection to the server:
      setdataperiod = 50000;
      Serial.println("setdata failed");
      //return 1;
    }
    lastdata = millis();
  }
  
  //waiting server answer
  if (clientsetData.available()) {
    while (clientsetData.available()) {
      char c = clientsetData.read();
      //if (c > 13) Serial.print(c);
      //else Serial.print(c, DEC);
      
      if (c == 13) {
        c = clientsetData.read();
        //Serial.print(c, DEC);
		if (c == 10) {
		  c = clientsetData.read();
		  //Serial.print(c, DEC);
		  if (c == 13) {
		    c = clientsetData.read();
		    //Serial.print(c, DEC);
		    if (c == 10) {
		      //c = clientgetData.read();
		      //Serial.print(c, DEC);
		      //data += c;
		      while (clientsetData.available()){
		        c = clientsetData.read();
		        data += c;
				//Serial.print(c);
			      }
			      Serial.println(data);
			      /*
			      	for (int i = 0; i < data.length(); i++) {
						Serial.print(data[i], DEC);
						if(data[i] == 10 || data[i] == 13) Serial.println("CRLF");
					}
					Serial.println();
					*/
			      Serial.println(millis() - lastdata);
			      Serial.println("setData DONE");
			      clientsetData.stop();
			      Serial.println(F("disconnected setdata"));
			      clientsetData.flush();
	            }
	          }
	        }
      }
    }
  }
  //return 0;
}

void RobotMonitor::getData(const String& tableoutLink, String& data) {
  if (millis() - lastgetdata > getdataperiod) {
    if (clientgetData.connect(server, 80)) {
    	data = "";
    	getdataperiod = 10000;
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
      //return 1;
    }
    lastgetdata = millis();
  }
  
  //waiting server answer
  if (clientgetData.available()) {
    while (clientgetData.available()) {
      char c = clientgetData.read();
      //if (c > 13) Serial.print(c);
      //else Serial.print(c, DEC);
      
      if (c == 13) {
        c = clientgetData.read();
        //Serial.print(c, DEC);
		if (c == 10) {
		  c = clientgetData.read();
		  //Serial.print(c, DEC);
		  if (c == 13) {
		    c = clientgetData.read();
		    //Serial.print(c, DEC);
		    if (c == 10) {
		      //c = clientgetData.read();
		      //Serial.print(c, DEC);
		      //data += c;
		      while (clientgetData.available()){
		        c = clientgetData.read();
		        data += c;
				//Serial.print(c);
			      }
			      Serial.println(data);
			      /*
			      	for (int i = 0; i < data.length(); i++) {
						Serial.print(data[i], DEC);
						if(data[i] == 10 || data[i] == 13) Serial.println("CRLF");
					}
					Serial.println();
					*/
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
}

void RobotMonitor::ping(const String& authHash, int& answer) {
  if (millis() - lastping >= pingperiod) {
    	//Serial.print(F(" ----------------------------------- connecting "));
    	//Serial.println(server);
    if (clientPing.connect(server, 80)) {
    	clientConection = true;
      pingperiod = 30000;
      clientPing.println("GET /device-acces/write/?hash=" + authHash + "&data=pingonline HTTP/1.0");
      clientPing.println("Host: portal.robotmonitor.com");
      clientPing.println("Connection: close");
      clientPing.println();
      Serial.println(F(" ----------------------------------- ping sent"));
      answer = 1;
      //return 1;
    }
    else {
      // if you didn't get a connection to the server:
      clientConection = false;
      pingperiod = 30000;
      Serial.println(F(" ----------------------------------- ping failed"));
      answer = 0;
    }
    lastping = millis();
   }

  if (clientPing.available()) {
      //Serial.println("available");
      char c = clientPing.read();
      //Serial.print(c);
      if (c == 10){
       	c = clientPing.read();
       	//Serial.print(c);
        if (c == '1'){
          Serial.print(F(" ----------------------------------- PING DONE: "));
          Serial.println(millis()-lastping);
          clientAnswered = true;
          clientPing.flush();
          answer = 2;
        }
      }
    } 
    
    //disconnect if success
    if ( clientConection == true && clientAnswered == true ) {
	    clientPing.stop();
	    clientConection = false;
	    clientAnswered = false;
        answer = 3;
        Serial.print(F(" ----------------------------------- disconnected ping: "));
        Serial.println(millis()-lastping);
    }
    
    //disconnect if timeout
    if ( clientConection == true && millis() - lastping >= 5000 ) {
	    clientPing.stop();
	    clientConection = false;
	    clientAnswered = false;
        answer = 4;
        Serial.print(F(" ----------------------------------- timeout ping: "));
    	Serial.println(millis()-lastping);
    }
}

void RobotMonitor::setIP(IPAddress& deviceIP){
	// SET THE LAST NUMBER OF IP ADDRESS
	// IP[3] - EEPROM byte 13, byte 14 for '#'
	//EEPROM.write(14, random(100,255)); //EEPROM.write(14, TrueRandom.randomByte());
	if (EEPROM.read(14) == '#') {
		deviceIP[3] = EEPROM.read(13);
		Serial.println("EEPROM IP: ");
		for (int i = 0; i < 4; i++) {
			Serial.print(deviceIP[i],DEC);
			if(i<3) Serial.print(".");
		}
	} else {
		deviceIP[3] = random(100, 255); //TrueRandom.randomByte();
		EEPROM.write(13, deviceIP[3]);
		EEPROM.write(14, '#');
		for (int i = 0; i < 4; i++) {
			Serial.print(deviceIP[i],DEC);
			if(i<3) Serial.print(".");
		}
	}
}

void RobotMonitor::setMAC(uint8_t *devicemac){
  // MAC
  //EEPROM.write(7, '*'); //EEPROM.write(7, TrueRandom.randomByte());
  if (EEPROM.read(7) == '#') {
    for (int i = 3; i < 6; i++) {
      devicemac[i] = EEPROM.read(i);
      Serial.print(devicemac[i],HEX);
      if(i<5) Serial.print(".");
    }
  } else {
    for (int i = 3; i < 6; i++) {
      devicemac[i] = random(100, 256); //TrueRandom.randomByte();
      EEPROM.write(i, devicemac[i]);
      Serial.print(devicemac[i],HEX);
      if(i<5) Serial.print(".");
    }
    EEPROM.write(7, '#');
  }

}

