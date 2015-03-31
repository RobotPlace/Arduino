/*
 created 20.Sept 2013
 by Petr Hubacek
 
 SKETCH PRINCIPLE:
 - keep permanent MAC address an the Arduino board
 - gets IP addres by DHCP
 - HTTP get request to post online activity
 
 COMMANDS:
  - none
  
 Circuit:
  - Arduino
  - Arduino Ethernet shield
 */
 
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <TrueRandom.h> //randomly generated MAC address
#include <RobotMonitor.h>

byte mac[6];

IPAddress robotIP(192, 168, 1, 180);
unsigned int portHTTP = 50180;
RobotMonitor rmdevice("putYourDeviceKeyHere");

void setup() {
  Serial.begin(57600);
// MAC
  //change the MAC address: uncomment the line below and upload the code again
  //avoid generate MAC every arduino restart: comment the line below and upload the code again
  //EEPROM.write(7, TrueRandom.randomByte());  
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
  
// IP
  // initialize the ethernet using DHCP:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP Failed");
    // initialize the ethernet NOT using DHCP:
    Ethernet.begin(mac, robotIP);
  } else {
    // print your local IP address:
    robotIP = Ethernet.localIP();
  }
  
  delay(100);

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

