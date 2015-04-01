#ifndef _ROBOTMONITOR_H_
#define _ROBOTMONITOR_H_

#include <Ethernet.h>

class RobotMonitor {
private:
  IPAddress server;
  EthernetClient client;
  String authHash;
  unsigned long pingperiod;
  unsigned long dataperiod;
  unsigned long lastping;
  unsigned long lastdata;
  boolean clientConection;
  boolean clientAnswered;
  String tableinLink;

public:
  void ping();
  boolean setData(const String& data);

  RobotMonitor(const String& authHash);
  ~RobotMonitor();
};

#endif // _ROBOTMONITOR_H_
