#include <SPI.h>

#include <Ethernet.h>
#include "RobotMonitor.h"

byte mac[] = {0xAC, 0xC2, 0xA3, 0xB8, 0x2E, 0x5B};

IPAddress robotIP(192,168,1,80);
unsigned int portHTTP = 8081;
String authHash = "authHash";
String tableinLink = "tableinLink";
String tableoutLink = "tableoutLink";

RobotMonitor rmdevice(authHash, tableinLink, tableoutLink);

String dataTabStart = "{%22millis%22:";
String dataTabEnd = "}";
String mydata = dataTabStart + 0 + dataTabEnd;
unsigned long dataLast = 0;

void makeData()
{
	if(millis()-dataLast > 10000){
		mydata = dataTabStart + String(millis()) + dataTabEnd;
		//Serial.print("mydata1:");
		//Serial.println(mydata);
		dataLast = millis();
	}
}

void setup() {
  Serial.begin(115200);
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
  makeData();
  rmdevice.ping();
  rmdevice.setData(mydata);
  rmdevice.getData();
}
   
