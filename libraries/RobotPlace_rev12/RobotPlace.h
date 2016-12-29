#ifndef _ROBOTPLACE_H_
#define _ROBOTPLACE_H_

#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <EEPROM.h>

class RobotPlace
{
private:
	//HTTP
	IPAddress server;
	byte devicemac[6];
	IPAddress deviceIP;
	String authHash;
	String clientType;
	EthernetClient clientInfo;
	EthernetClient clientsetData;
	EthernetClient clientgetData;
	unsigned long lastInfo;
	unsigned long lastInfoOnline;
	boolean clientConection;
	boolean clientAnswered;

	// TCP and UDP
	char serverLive[21];
	int serverPort;
	
	//Live_TCP
	EthernetClient clientTCP;
	String loginLink;
	String data;
	String bufferTCP;
	unsigned long status;
	unsigned long serverTCPstatus;
	
	//Live_UDP
	EthernetUDP Udp;
	unsigned long serverUDPstatus;

public:
	//HW Setup
	//void setIP(IPAddress& deviceIp);
	void setMAC(uint8_t *devicemac);
	// *** HTTP requests ***
	// read and write information from robot profile and tadatable
	void info( const String& infoType, String& data, String& _answer);
	// *** read and write tadatables in json format ***
	void dataTableWrite( const String& tableinLink, String& data, String& answer);
	void dataTableRead( const String& tableoutLink, String& data, String& answer);
	// *** Live_TCP ***
	// initiate and connect TCP cosket to RobotPlace server
	void LiveTCP_init( int& _status ); // connect to the server data channel
	// read TCP
	void LiveTCP_read( String& data, int& _status );  // read data from data channel
	// wrtie to TCP
	void LiveTCP_write( String& data, int& _status ); // write data to data channel

	// *** Live_UDP ***
	// initiate and connect UDP cosket to RobotPlace server
	void LiveUDP_init( int& _status ); // connect to the server data channel
	// read UDP
	void LiveUDP_read( String& data, int& _status );  // read data from data channel
	// send UDP
	void LiveUDP_write( String& data, int& _status ); // write data to data channel
	
	RobotPlace( const String& authHash, uint8_t *devicemac, const IPAddress& deviceIp);
	~RobotPlace();
};

#endif // _ROBOTPLACE_H_