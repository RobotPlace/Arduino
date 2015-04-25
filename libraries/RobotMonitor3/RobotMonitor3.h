#ifndef _ROBOTMONITOR_H_
#define _ROBOTMONITOR_H_

#include <Ethernet.h>
#include <EEPROM.h>
//#include <TrueRandom.h> //randomly generated MAC address

class RobotMonitor {
private:
  IPAddress server;
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

public:
  int ping(const String& authHash);
  int setData(const String& tableinLink, const String& data);
  int getData(const String& tableoutLink, String& data);
  void setIP(IPAddress& deviceIp);
  void setMAC(uint8_t *devicemac);

  RobotMonitor(uint8_t *devicemac, const IPAddress& deviceIp);
  ~RobotMonitor();
};

#endif // _ROBOTMONITOR_H_
