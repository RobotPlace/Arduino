/*
 Web client robotmonitor ping online status

 SKETCH PRINCIPLE:
 - HTTP get request to post online activity
 
 COMMANDS:
  - none
 
 Circuit:
 * Arduino
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2013
 by Petr Hubacek
 
*/

#include <SPI.h>
#include <Ethernet.h>

// --------------- TrueRandom sets this, so no default is needed here.
byte mac[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF};

/****** TO MODIFY as needed *********************/
IPAddress robotIP(192,168,1,180);
unsigned int portHTTP = 50180;

String authHash = "putYourDeviceAuthHashKeyHere";
String robotName = "HTTP_pingonline";
String sourceFile = "http_ping_easy_nolibrary.ino";

/***************************************************/

unsigned long pingperiod = 5000;
unsigned long lastping = 0;
boolean clientConection = false;
boolean clientAnswered = false;

IPAddress server(46,28,111,207); // www.robotmonitor.com

EthernetClient client;

void setup() {
  Serial.begin(57600);
  Ethernet.begin(mac, robotIP);
  delay(100);
  Serial.print("robotName: ");
  Serial.println(robotName);
  Serial.print("sourceFile: ");
  Serial.println(sourceFile);
  Serial.print("robotIP: ");
    for (int i = 0; i <= 3; i++) {
      Serial.print(robotIP[i], DEC);
      if (i < 3) Serial.print(".");
    }
  Serial.println();
  Serial.print("portHTTP: ");
  Serial.println(portHTTP);
}

void loop()
{
	ping();
}

void ping()
{
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





