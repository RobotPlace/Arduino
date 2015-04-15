#ifndef _ROBOTMONITOR_H_
#define _ROBOTMONITOR_H_

#include <Ethernet.h>
#include <EEPROM.h>
//#include <TrueRandom.h> //randomly generated MAC address

class RobotMonitor {
private:
  IPAddress server;
  String authHash;
  String tableinLink;
  String tableoutLink;
  byte devicemac[6];
  IPAddress deviceIP;
  EthernetClient clientPing;
  EthernetClient clientData;
  EthernetClient clientgetData;
  unsigned long pingperiod;
  unsigned long dataperiod;
  unsigned long getdataperiod;
  unsigned long lastping;
  unsigned long lastdata;
  unsigned long lastgetdata;
  String getDataBuffer;

public:
  void ping();
  void setData(const String& data);
  void getData();
  void setIP();
  void setMAC();

  RobotMonitor(const String& authHash, const String& tableinLink, const String& tableoutLink, uint8_t *devicemac, const IPAddress& deviceIp, const String& getDataBuffer);
  ~RobotMonitor();
};

#endif // _ROBOTMONITOR_H_
