#ifndef _ROBOTMONITOR_H_
#define _ROBOTMONITOR_H_

#include <Ethernet.h>

class RobotMonitor {
private:
  IPAddress server;
  EthernetClient client;
  String authHash;
  unsigned long pingperiod;
  unsigned long lastping;
  boolean clientConection;
  boolean clientAnswered;
  
public:
  void ping();
  
  RobotMonitor(const String authHash);
  ~RobotMonitor();
};

#endif // _ROBOTMONITOR_H_
