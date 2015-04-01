#ifndef _ROBOTMONITOR_H_
#define _ROBOTMONITOR_H_

#include <Ethernet.h>

class RobotMonitor {
private:
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

public:
  void ping();
  void setData(const String& data);
  void getData();
  
  RobotMonitor(const String& authHash, const String& tableinLink, const String& tableoutLink);
  ~RobotMonitor();
};

#endif // _ROBOTMONITOR_H_
