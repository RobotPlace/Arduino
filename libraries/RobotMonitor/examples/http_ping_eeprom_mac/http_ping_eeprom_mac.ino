/*
 Web client robotmonitor ping online status
 
 Circuit:
 * Arduino
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2013
 by Petr Hubacek
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <TrueRandom.h> //randomly generated MAC address
#include <RobotMonitor.h> //proxy's library

// --------------- TrueRandom sets this, so no default is needed here.
byte mac[6];

/****** TO MODIFY As you want *********************/
IPAddress robotIP(192,168,1,199);
unsigned int portHTTP = 51990;

/****** TO MODIFY ACCORDING ROBOTMONITOR **********/

String authHash = "24c1b87d300a3f463eb1ae06ebfbaf65";
//RobotMonitor SETUP
String robotName = "#76 EX0_HTTP_pingonline";
String sourceFile = "Demo_ping";
String robotType = "Demo";
String robotOwner = "Petr Hubacek";

/***************************************************/

unsigned long pingperiod = 5000;
unsigned long lastping = 0;
boolean clientConection = false;
boolean clientAnswered = false;

IPAddress server(46,28,111,207); // www.robotmonitor.com

RobotMonitor rmdevice(authHash);

void setup() {
  Serial.begin(57600);
  //EEPROM.write(7, 99);  //uncomment to change MAC address
  if (EEPROM.read(7) == '#') {
    for (int i = 0; i < 6; i++) {
      mac[i] = EEPROM.read(i);
      Serial.print(mac[i],HEX);
		Serial.print(".");
    }
  } else {
    for (int i = 0; i < 6; i++) {
      mac[i] = TrueRandom.randomByte();
      EEPROM.write(i, mac[i]);
    }
    EEPROM.write(7, '#');
  }
	Serial.println();
  // start the Ethernet connection:
  Ethernet.begin(mac, robotIP);

  // give the Ethernet shield a second to initialize:
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
  rmdevice.ping();
}
