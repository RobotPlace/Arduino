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

// basic setup and info
  void ping(const String& authHash, int& answer);
  void setIP(IPAddress& deviceIp);
  void setMAC(uint8_t *devicemac);

//Device config setup
	void DeviceProfile_write( String authHash, String data );
	void DeviceProfile_read( String authHash, String data );
	void DeviceFile_read( String authHash, String FileName );

//Device Data setup	
  void DataTable_write(const String& tableinLink, String& data); //setData
  void DataTable_read(const String& tableoutLink, String& data); //getData

//read commands from Device DataTables saved by a user
	void HTTPCloud_read( String tableoutLink, String data );
	void HTTPCloud_write( String tableinLink, String data );
	void LiveDataServer_read( String data );
	
// connect TCP and keep logged
// status 0=notconnected, 1=login, 2=disconected by server
// 10=0 listeners, 11=1 listener, 12=2 listeners,..., 22=12 listeners...
	void LiveTCP_on( String loginLink, int status );
	void LiveTCP_read( String data, int status );
	void LiveTCP_write( String data, int status );
	
// connect UDP and keep logged
// status 0=notconnected, 1=login, 2=disconected by server
// 10=0 listeners, 11=1 listener, 12=2 listeners,..., 22=12 listeners...
	void LiveUDP_on( String loginLink, int status );
	void LiveUDP_write( String data, int status );
	void LiveUDP_read( String data, int status );
	
  RobotMonitor(uint8_t *devicemac, const IPAddress& deviceIp);
  ~RobotMonitor();
};

#endif // _ROBOTMONITOR_H_
