#ifndef _ROBOTMONITOR_H_
#define _ROBOTMONITOR_H_

#include <Ethernet.h>
#include <EEPROM.h>
//#include <TrueRandom.h> //randomly generated MAC address

class RobotMonitor {
private:
  byte devicemac[6];
  IPAddress deviceIP;
  IPAddress server;
  EthernetClient clientPing;
  EthernetClient clientData;
  EthernetClient clientgetData;
  String authHash;
  String tableinLink;
  String tableoutLink;
  unsigned long pingperiod;
  unsigned long dataperiod;
  unsigned long getdataperiod;
  unsigned long lastgetdata;
  unsigned long lastping;
  unsigned long lastdata;
  String getDataBuffer;

public:
  void ping();
  void setData(const String& data);
  void getData();
  void setIP();
  void setMAC();

  RobotMonitor(const String& authHash, const String& tableinLink, const String& tableoutLink);
  ~RobotMonitor();
};

#endif // _ROBOTMONITOR_H_
