#include <SPI.h>

#include <Ethernet.h>
#include <RobotMonitor.h>

byte mac[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF};

IPAddress robotIP(192, 168, 1, 180);
unsigned int portHTTP = 50180;
RobotMonitor rmdevice("deviceKey");

boolean dataNew = false;
String dataTabStart = "{%22millis%22:";
String dataTabEnd = "}";
String mydata = dataTabStart + 0 + dataTabEnd;

void makeData()
{
  mydata = dataTabStart + String(millis()) + dataTabEnd;
  Serial.print(mydata);
  dataNew = true;
}

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
  makeData();
  if (dataNew && rmdevice.setData(mydata)) {
     dataNew = false; 
  }
}
   
