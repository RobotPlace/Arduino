// r1 2015_05_20 functional:
//	void setIP( IPAddress deviceIp );
//	void setMAC( int devicemac );
//	void info( "online", infoData);
//	void info( "server", infoData);

// r2 2015_05_21 functional:
//	void DataTable_write( String tableinLink, String data );
//	void DataTable_read( String tableoutLink, String data );

// r3 2015_05_22 functional:
//	void info( "getprofile", infoData);
//	void info( "setprofile", infoData);

// r4 2015_05_29 functional:
//	void info( "tablereadkey", dataTableName);
//	void info( "tablewritekey", dataTableName);
//  info() process until server answers or timout

// r8 2015_06_15 functional:
// LiveTCP_init() - to connect, login and keep
// LiveTCP_read() - to read incomming messages
// LiveTCP_write()- to write messages

// r10 2015_06_17 functional:
// LiveUDP_init() - to initialise and login
// LiveUDP_read() - to read incomming messages
// LiveUDP_write()- to write messages

// r12 2015_07_20
// comment setIP(IPAddress& deviceIP)
// use progmem to store strings

#include "RobotPlace.h"

// v3 added HTTP requests for dataTables
// WebSocket communication works independently on robotplace sollution
// v4 added TCP LiveDate

/* robotplace lib functions

	robotplace(int devicemac, IPAddress deviceIp );
	void setIP( IPAddress deviceIp );
	void setMAC( int devicemac );
	
	void info( String infoType, String infoData);

	void DataTable_write( String tableinLink, String data );
	void DataTable_read( String tableoutLink, String data );
	
	void DeviceFile_read( String FileName );

	void LiveTCP_init( String loginLink, int status );
	void LiveTCP_read( String data, int status );
	void LiveTCP_write( String data, int status );

	void LiveUDP_connect( String loginLink, int status );
	void LiveUDP_write( String data, int status );
	void LiveUDP_read( String data, int status );
*/

RobotPlace::RobotPlace( const String& _authHash, byte *devicemac, const IPAddress& deviceIP)
	: server(89, 221, 208, 211),
	  //infoType(),
	  authHash(_authHash),
	  //http
	  deviceIP(deviceIP),
	  clientInfo(),
	  clientType(),
	  lastInfo(0),
	  lastInfoOnline(0),
	  clientConection(false),
	  clientAnswered(false),
	  //LiveData
	  clientTCP(),
	  serverLive(), // us2.robotplace.com
	  serverPort(8080),
	  loginLink(),
	  data(),
	  status(),
	  serverTCPstatus(0),
	  bufferTCP()
	  //UDP
	  
{
	memcpy(&this->devicemac, devicemac, 6);
}

RobotPlace::~RobotPlace()
{
}

/*
void RobotPlace::setIP(IPAddress& deviceIP)
{
	// SET THE LAST NUMBER OF IP ADDRESS
	// THIS CAN BE REMOVED FROM LIB IF ONLY DHCP WILL USED
	// IP[3] - EEPROM byte 13. Byte 14 for '#'
	// EEPROM.write(14, '*');
	if (EEPROM.read(14) == '#')
	{
		deviceIP[3] = EEPROM.read(13);
	}
	else
	{
		deviceIP[3] = random(100, 255);
		EEPROM.write(13, deviceIP[3]);
		EEPROM.write(14, '#');
	}
}
*/
void RobotPlace::setMAC(uint8_t *devicemac)
{
	// MAC - EACH ARDUINO SHALL HAVE ITS OWN MAC TO AVOID PROBLEMS DURING DHCP USE
	// MAC - EEPROM byte 0 to 6. Byte 7 for '#'
	// EEPROM.write(7, '*');
	if (EEPROM.read(7) == '#')
	{
		for (int i = 2; i < 6; i++)
		{
			devicemac[i] = EEPROM.read(i);
		}
	}
	else
	{
		for (int i = 2; i < 6; i++)
		{
			devicemac[i] = random(100, 256);
			EEPROM.write(i, devicemac[i]);
		}
		EEPROM.write(7, '#');
	}
}

// info() Function will reads all for Device Profile and DataTables via requests
// => TO IMPROVE: MAKE SEVERAL REQUESTS SIMULTANEOUSLY AND ATTRIBUTE SERVER ANSWERS TO THE RIGHT REQUEST
// => TO IMPROVE: TOO MANY STRING VARIABLES ...
// infoType strings = { online, server, getprofile, setiptcp, setipudp, tablereadkey, tablewritekey, message, datachannel, tableread, tablewrite }
// infoType integer = { setporttcp, setportudp }
// HTTP REQUESTS EXAMPLES:
// http://portal.robotplace.com/device-acces/read?hash=3a0d539d0e019ce01a518931193fe2df&get=tablewritekey:TABLENAME
// http://portal.robotplace.com/device-acces/read?hash=3a0d539d0e019ce01a518931193fe2df&get=tablereadkey:TABLENAME
// http://portal.robotplace.com/device-acces/write/?hash=&60474c9c10d7142b7508ce7a50acf414&data={%22online%22:%22201302091859%22}
void RobotPlace::info( const String& _infoType, String& _data, String& _answer)
{

	// _answer = { 0=failed, 1=sent, 2=waiting, 3=no time, 4=timeout, receiveddata }

	// process only if no other request is waiting for server answer
	String msg = "";
	if ( clientConection == false )
	{
		if ( ( _infoType == "online" && millis() - lastInfoOnline >= 600000 )  ||  ( _infoType != "online" && millis() - lastInfo >= 1000 ) )
		{
			// ONLINE || SERVER
			if (clientInfo.connect(server, 80))
			{
				//Serial.println("4");
				clientConection = true;
				clientType = _infoType;

				String getLink = "GET ";
				String getdevice_acces = "/device-acces/";
				String getLinkRead = "read?hash=" + authHash;
				String getLinkWrite = "write/?hash=" + authHash + "&data=";
				String left = "{%22";
				String mid = "%22:";
				String right = "}";
				String rightHTTP = " HTTP/1.0";

				// Read info
				if ( _infoType.equals("getprofile") ) 			getLink = getLink + getdevice_acces + getLinkRead + " HTTP/1.0";
				else if ( _infoType.equals("tablereadkey" ) ) 	getLink = getLink + getdevice_acces + getLinkRead + "&get=" + _infoType + ":" + _data + rightHTTP;
				else if ( _infoType.equals("tablewritekey") ) 	getLink = getLink + getdevice_acces + getLinkRead + "&get=" + _infoType + ":" + _data + rightHTTP;
				else if ( _infoType.equals("tableread") ) 		getLink = getLink + _data + rightHTTP;
				else if ( _infoType.equals("datachannel") ) 	getLink = getLink + getdevice_acces + getLinkRead + "&get=pingserver" + ":" + _data + rightHTTP;
				// Write info
				else if ( _infoType.equals("online") ) 			getLink = getLink + getdevice_acces + getLinkWrite + "ping" + _infoType + rightHTTP;
				else if ( _infoType.equals("server") ) 			getLink = getLink + getdevice_acces + getLinkWrite + "ping" + _infoType + rightHTTP;
				else if ( _infoType.equals("setiptcp") ) 		getLink = getLink + getdevice_acces + getLinkWrite + left + "iptcp" 	+ mid + _data + right + rightHTTP;
				else if ( _infoType.equals("setporttcp") ) 		getLink = getLink + getdevice_acces + getLinkWrite + left + "porttcp" 	+ mid + _data + right + rightHTTP;
				else if ( _infoType.equals("setipudp") ) 		getLink = getLink + getdevice_acces + getLinkWrite + left + "ipudp" 	+ mid + _data + right + rightHTTP;
				else if ( _infoType.equals("setportudp") ) 		getLink = getLink + getdevice_acces + getLinkWrite + left + "portudp" 	+ mid + _data + right + rightHTTP;
				else if ( _infoType.equals("message") ) 		getLink = getLink + getdevice_acces + getLinkWrite + _data + rightHTTP;
				else if ( _infoType.equals("tablewrite") ) 		getLink = getLink + _data + rightHTTP;

				Serial.println();
				Serial.println(getLink);
				Serial.println();
				clientInfo.println(getLink);
				clientInfo.println("Host: portal.robotplace.com");
				clientInfo.println("Connection: close");
				clientInfo.println();
				_answer = "1";
			}
			else
			{
				// if you didn't get a connection to the server:
				clientConection = false;
				_answer = "0";
			}
			if ( _infoType == "online") lastInfoOnline = millis();
			lastInfo = millis();
		}
		else
		{
			_answer = "3";
		}
	}
	else
	{
		_answer = "2";
	}

	// wait server answer until recieved or timeout
	while ( clientConection == true )
	{
		if (clientInfo.available())
		{
			String buffer = "";
			while (clientInfo.available())
			{
				// content is enclosed inside these charecters 13101310CONTENT1310
				char c = clientInfo.read();
				if (c == 13)
				{
					c = clientInfo.read();
					if (c == 10)
					{
						c = clientInfo.read();
						if (c == 13)
						{
							c = clientInfo.read();
							if (c == 10)
							{
								int i = 0;
								//save answer to the server
								while ( clientInfo.available() )
								{
									c = clientInfo.read();
									if( clientType == "online" )
									{
										msg = String(c);
									}
									else if( clientType == "server" )
									{
										serverLive[i++] = c;
										if(serverLive[0] == '2')
										{
											memset(serverLive, 0, 21);
											serverLive[0] = '2';
										}
										msg = String(serverLive);
									}
									else {
										msg += c;
									}
								}
								if( clientType == "datachannel" ){
									loginLink = msg;
								}
								clientAnswered = true;
							}
						}
					}
				}
			}
		}

		//disconnect if success
		if ( clientConection == true && clientAnswered == true )
		{
			clientInfo.stop();
			clientConection = false;
			clientAnswered = false;
			_answer = msg;
		}
		//disconnect if timeout
		else if ( clientConection == true )
		{
			if( millis() - lastInfo >= 5000 )
			{
				clientInfo.stop();
				clientConection = false;
				clientAnswered = false;
				_answer = "4";
			}
		}
	}

}

// send json data to a datatable to the server
void RobotPlace::dataTableWrite( const String& tableinLink, String& data, String& answer )
{

	data = "/device/in/" + tableinLink + "/?data=" + data;
	RobotPlace::info( "tablewrite", data, answer);

}

// read json data from a datatable on the server
void RobotPlace::dataTableRead( const String& tableoutLink, String& data, String& answer )
{

	data = "/device/out/" + tableoutLink + "/?data=" + data;
	RobotPlace::info( "tableread", data, answer);

}

// Connect and login to the TCP channel on the server
// S T A T U S
// LiveTCP_init - 0 not connected / 1 connected
// LiveTCP_write   - 0 not connected/not sent => try 3x LiveTCP_init and resend / 4 writing complete
// LiveTCP_read    - 5 reading / 6 reading complete / 10 no listenner / 11 one listenner (32 = 22 listenners)

// T C P  L O G I N
//------------------------------
// connect TCP and keep logged - status 0=disconnected, 1=connected, 2=logged, 3=disconected by server,
// 4=writing complete, 5=reading, 6=reading complete
// 10=0 listeners, 11=1 listener, 12=2 listeners,..., 22=12 listeners...
void RobotPlace::LiveTCP_init( int& _status )
{
	// Commands:
	// 0 = disconnect
	// 1 = connect
	
	// Main status:
	// 0 = not processed - wrong loginLink
	// 1 = processed - connected
	
	// Secondary status:
	// >1000 = disconnection duration

	if( loginLink.length() > 10 )
	{

		// C O N N E C T

		if( !clientTCP.connected() && _status == 1 )
		{
			if( millis() - (serverTCPstatus*1000) > 2000 ) //try connect every 2s if unsuccessfull login
			{
				Serial.println("Connecting server...");
				
				if (clientTCP.connect("us2.robotplace.com", serverPort) )
				{
					Serial.println("TCP OK");
					for(int i = 0; i < 20; i++) Serial.print("!");
					clientTCP.println(loginLink);
					clientTCP.println("init");
					serverTCPstatus = 1;  //connected
				}
				else
				{
					// if you didn't get a connection to the server:
					Serial.println("TCP failed");
					serverTCPstatus = millis()/1000; // last millis when disconnected
				}
			} else {
				Serial.println("TCP reconnect delay");
			}
			
		// D I S C O N N E C T
		
		}
		else if ( clientTCP.connected() && _status == 0 )     // clientTCP.connected()
		{
			clientTCP.stop();
			serverTCPstatus = millis()/1000; // last millis when disconnected
			Serial.println("TCP stopped");
		}
		else if ( !clientTCP.connected() && _status == 0 )
		{
			Serial.println("TCP disconnected");
			serverTCPstatus = millis()/1000; // last millis when disconnected
		}
		else if ( clientTCP.connected() && _status == 1 )
		{
			Serial.println("TCP persist");
			serverTCPstatus = 1;
		}
	}
	else {
		Serial.println(loginLink);
		Serial.println(loginLink.length());
		Serial.println("TCP_init wrong loginLink ");
		serverTCPstatus = 0;
	}
	
	_status = serverTCPstatus;

}

// T C P  W R I T E
//------------------------------
void RobotPlace::LiveTCP_write( String& _data, int& _status )
{
	// Main status:
	// 0 = not processed
	// 1 = processed
	
	int cmd = 1; //connect and login
	RobotPlace::LiveTCP_init( cmd );
	
	if ( cmd == 1 )
	{
		clientTCP.print(_data);
		clientTCP.print("\r\n");
		_status = 1;
	}
	else {
		int cmd = 1; //connect and login
		RobotPlace::LiveTCP_init( cmd );
		_status = 0; // TCP write not performed
	}
}

// T C P  R E A D
//------------------------------
void RobotPlace::LiveTCP_read( String& _data, int& _status )
{
	
	// connect UDP and keep logged
	// Main status:
	// 0 = not processed - disconnected - nothing received
	// 1 = processed - connected - reading complete
	// Secondary status:
	// 2 = reading
	// 10 = 0 listeners
	// 11 = 1 listener
	// 12 = 2 listeners
	// ...
	// 22 = 12 listeners...
	
	// if there are incoming bytes available
	// from the server, read them
	boolean bufferTCPcomplete = false;

	int cmd = 1; //connect and login
	RobotPlace::LiveTCP_init( cmd );

	if ( cmd == 1 )
	{
		_status = 1; // connected
		
		if(clientTCP.available())
		{
			char c;
			_status = 2; // reading
	
			while(clientTCP.available())
			{
				c = clientTCP.read();
				bufferTCP += c;
			}

			bufferTCPcomplete = true;
			Serial.println();
			Serial.println("read:");
			Serial.println(bufferTCP);

		} else {
			_data = bufferTCP;
			_status = 0;
		}
	
		if( bufferTCPcomplete == true )
		{

			// check if we got "0"
			if( bufferTCP[0] == 48 && bufferTCP.length() == 1 )  // 48 = "0"
			{
				Serial.println("read: 0");
			}
			
			if ( bufferTCP[1] == 58 || bufferTCP[0] == 58 ) // 58 = ":"
			{
				int separ1 = bufferTCP.indexOf(58, 0); // 58 = ":"
				int separ2 = bufferTCP.indexOf(32, 0); // 32 = " "
				String list = bufferTCP.substring(0, separ1);
				String devi = bufferTCP.substring(separ1, separ2);
				int listeners = (int(list[0]) - 48) * 10 + int(list[0]) - 48;
				int devices = (int(devi[0]) - 48) * 10 + int(devi[0]) - 48;
				_status = listeners;
			} else {
				_data = bufferTCP;
				_status = 1;
			}
			
			bufferTCP = "";
			bufferTCPcomplete = false;
		}
	}
	else {
		cmd = 1; //connect and login
		RobotPlace::LiveTCP_init( cmd );
		_status = 0;  // TCP read not performed
	}
	
}

// Login to the UDP channel on the server
// U D P   L O G I N
//------------------------------
// connect and keep logged - status 0=disconnected, 1=connected, 2=logged, 3=disconected by server,
// 4=writing complete, 5=reading, 6=reading complete
// 10=0 listeners, 11=1 listener, 12=2 listeners,..., 22=12 listeners...
void RobotPlace::LiveUDP_init( int& _status )
{
	// Commands:
	// 0 = disconnect
	// 1 = connect
	
	// Main status:
	// 0 = not processed - wrong loginLink
	// 1 = processed - connected
	
	// Secondary status:
	// >1000 = disconnection duration

	if( loginLink.length() > 10 )
	{

		// C O N N E C T
		unsigned int localPort = 51870;      // local port to listen on
		Udp.begin(localPort);
		serverUDPstatus = 1;
		Serial.println("UDP initialised");
		Serial.println("UDP logging...");
		int _cmd = 1;
		RobotPlace::LiveUDP_write(loginLink, _cmd);

	}
	
	_status = serverTCPstatus;
}

// U D P   W R I T E
//------------------------------
void RobotPlace::LiveUDP_write( String& _data, int& _status )
{
	// Main status:
	// 1 = processed
	
	// send a reply, to the IP address and port that sent us the packet we received
	//Serial.println("send UDP cmd");
	//Serial.println(_data);
	//Serial.println(_data.length());
	char _msg[128];
	memset(_msg, 0, 128);
	//convert String to a char buffer
	int j = _data.length()+1;
	_data.toCharArray(_msg, j);

	//Serial.println("_msg = ");
	//Serial.println(_msg);
	String sUDP = "us2.robotplace.com";
	int k = sUDP.length()+1;
	sUDP.toCharArray(serverLive, k);
	//Serial.print("serverLive = ");
	//Serial.println(serverLive);
	Udp.beginPacket(serverLive, serverPort);
	Udp.write(_msg);
	Udp.write('\r\n');
	Udp.endPacket();

	Serial.println("UDP sent");
	_status = 1;

}

// U D P   R E A D
//------------------------------
void RobotPlace::LiveUDP_read( String& _data, int& _status )
{
	// Main status:
	// 1 = processed
	// 2 = nothing received
	// 10 = zero listeners
	// 12 = 2 listeners
	
	// buffers for receiving and sending data
	const int maxSize = 128;
	char packetBuffer[maxSize]; //buffer to hold incoming packet, //UDP_TX_PACKET_MAX_SIZE
	
	// if there's data available, read a packet
	if ( Udp.parsePacket() > 0 ) {
		// read the packet into packetBufffer
		Udp.read(packetBuffer, maxSize); //UDP_TX_PACKET_MAX_SIZE
		_data = String(packetBuffer);
		
		// check if we got "0"
		if( _data[0] == 48 && _data.length() == 1 )  // 48 = "0"
		{
			Serial.println("UDP read: 0");
			_status = 10;
		}
		
		if ( _data[1] == 58 || _data[2] == 58 ) // 58 = ":"
		{
			int separ1 = _data.indexOf(58, 0); // 58 = ":"
			int separ2 = _data.indexOf(32, 0); // 32 = " "
			String list = _data.substring(0, separ1);
			String devi = _data.substring(separ1, separ2);
			int listeners = (int(list[0]) - 48) * 10 + int(list[0]) - 48;
			int devices = (int(devi[0]) - 48) * 10 + int(devi[0]) - 48;
			_status = listeners;
		}

		_status = 1;
	} else {
		_status = 2;
	}
	delay(10);
}
