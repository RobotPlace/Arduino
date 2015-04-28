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
  EthernetClient clientsetData;
  EthernetClient clientgetData;
  unsigned long pingperiod;
  unsigned long setdataperiod;
  unsigned long getdataperiod;
  unsigned long lastping;
  unsigned long lastdata;
  unsigned long lastgetdata;
  boolean clientConection;
  boolean clientAnswered;

public:
  void ping(const String& authHash, int& answer);
  void setData(const String& tableinLink, String& data);
  void getData(const String& tableoutLink, String& data);
  void setIP(IPAddress& deviceIp);
  void setMAC(uint8_t *devicemac);

  RobotMonitor(uint8_t *devicemac, const IPAddress& deviceIp);
  ~RobotMonitor();
};

#endif // _ROBOTMONITOR_H_
