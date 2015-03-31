/*
 created 20.Sept 2013
 by Petr Hubacek
 
 SKETCH PRINCIPLE:
 - HTTP get request to post online activity
 
 COMMANDS:
  - none
  
 Circuit:
  - Arduino
  - Arduino Ethernet shield
 */
 
#include <SPI.h>
#include <Ethernet.h>
#include <RobotMonitor.h>

byte mac[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF};

IPAddress robotIP(192, 168, 1, 180);
unsigned int portHTTP = 50180;
RobotMonitor rmdevice("putYourDeviceKeyHere");

void setup() {
  Serial.begin(57600);
  Ethernet.begin(mac, robotIP);
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

