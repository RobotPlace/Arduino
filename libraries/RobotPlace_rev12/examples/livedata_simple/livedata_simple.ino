/*
 Web client robotplace:
 1) ping online status
 2) get table readkey and writekey
 3) read a table
 4) write to a table
 5) read/write device profile info incl. messages
 6) TCP liveData connect, login, read, write
 7) TCP liveData read, write data
 8) UDP liveData connect, login, read, write
 9) added Progmem to store strings (first step to reduce code size)
 
 ISSUES
 1) working, stability to verify
 2) working, stability testing --- working for 12h without an interruption
 3) working, stable, working for 27 hours - ok (strange one case of non detection on LAN disconnection)
 4) better TCP disconnection and UDP logout detection

 // function rmdevice.info( infoType, data ) is not returning a value until request finished or timeout
 // example on the new library use

 created 18 Mars 2015
 by Petr Hubacek
*/

#define DEVMODE 0

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>		// UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <EEPROM.h>
#include "RobotPlace5.h"

/****** TO MODIFY as needed *********************/
// device setup
byte devicemac[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF};
IPAddress deviceIP(192, 168, 1, 180);
unsigned int portHTTP = 80;

// device login setup
String deviceAuthKey = "9aa5954b23f97a5954b23f97238636aaaa48b4aa8";

// make robotplace instance
RobotPlace rmdevice(deviceAuthKey, devicemac, deviceIP);

String infoType = "";
String data = "";
String answer = "";
int status = 0;
String LiveDataLoginLink = "";
unsigned long lastloop = 0;
int loopperiod = 0;
/***************************************************/

void setup()
{
	//if (DEVMODE)
	{
		Serial.begin(57600);
		delay(2000);
	}


	rmdevice.setMAC(devicemac);

	//if (DEVMODE)
	{
		for (int i = 0; i < 6; i++)
		{
			Serial.print(devicemac[i], HEX);
			if(i < 5) Serial.print(".");
		}
		Serial.println();

		Serial.println("DHCP");
	}
	
	if (Ethernet.begin(devicemac) == 0)
	{
		//Serial.println("DHCP Failed");
		//Serial.println("Device STOP");
		for(;;)
			;
	}

	for (int i = 0; i < 4; i++)
	{
		//Serial.print(Ethernet.localIP()[i], DEC);
		if(i < 3) Serial.print(".");
	}
	Serial.println();

	// G E T   C H A N N E L   L O G I N   L I N K
	rmdevice.info( infoType = "datachannel", data = "LiveData", answer );

	//if (DEVMODE)
	{
		Serial.print("info answer = ");
		Serial.println(answer);

		Serial.println("TCP_init");
	}
	status = 1; // we want to connect or to keep connected, 0 = we want disconnect
	rmdevice.LiveTCP_init( status ); // connect to the server data channel

	//if (DEVMODE)
	{
		Serial.print("TCP_init answer = ");
		Serial.println(status);
		Serial.println("UDP_init");
	}
	status = 1;
	rmdevice.LiveUDP_init( status );

	if (DEVMODE)
	{
		Serial.print("UDP_init answer = ");
		Serial.println(status);
		Serial.println("End Setup");
	}
}

void loop()
{

	status = 1;
	rmdevice.LiveUDP_init( status );
	Serial.print("TCP_init answer = ");
	Serial.println(status);
		
	if( status == 1 )
	{
		
		if(millis() - lastloop > loopperiod)
		{

			// T C P   R E A D   F R O M   S E R V E R

			rmdevice.LiveTCP_read( data, status );  // read data from data channel
			Serial.print("TCP_read status = ");
			Serial.println(status);
			Serial.print("TCP_read data = ");
			Serial.println(data);

			// T C P   S E N D   D A T A   T O   T H E   S E R V E R

			Serial.print("TCP_write = ");
			data = String(millis());
			Serial.println(data);
			rmdevice.LiveTCP_write( data, status ); // write data to data channel
			Serial.print("TCP_write status = ");
			Serial.println(status);

			// U D P   S E N D   D A T A   T O   T H E   S E R V E R

			Serial.println();
			Serial.print("UDP_write = ");
			data = String(millis());
			rmdevice.LiveUDP_write( data, status );
			Serial.print("UDP_write status = ");
			Serial.println(status);

			// U D P   R E A D   F R O M   S E R V E R

			Serial.print("UDP_read = ");
			rmdevice.LiveUDP_read( data, status );
			Serial.println(data);
			Serial.print("UDP_read status = ");
			Serial.println(status);

			Serial.println("100 ms delay...");
			Serial.println();
			Serial.println();
			lastloop = millis();

			if(millis() - lastloop > 1000)
			{
				if(loopperiod <= 50)
				{
					loopperiod = loopperiod * 1.1;
					Serial.println("100 ms delay...");
					Serial.println();
				}
				else if(loopperiod >= 2000)
				{
					loopperiod = loopperiod / 1.1;
					Serial.println("100 ms delay...");
					Serial.println();
				}
			}

		}
	}
}

