/*
 Web client robotmonitor:
 1) ping online status
 2) get table readkey and writekey
 3) read a table
 4) write to a table
 5) read/write device profile info incl. messages
 6) TCP liveData connect, login, read, write
 7) TCP liveData read, write data
 
 ISSUES
 1) working, stability to verify
 
 // function rmdevice.info( infoType, data ) is not returning a value until request finished or timeout
 // example on the new library use

 created 18 Mars 2015
 by Petr Hubacek
*/

#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include "RobotMonitor5.h"

byte mac[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF};

/****** TO MODIFY as needed *********************/
byte devicemac[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF};
IPAddress deviceIP(192,168,1,180);
unsigned int portHTTP = 80;
String deviceAuthKey = "874197e3aaa5954b23f97238636aa48b4aa8";
RobotMonitor rmdevice(deviceAuthKey, devicemac, deviceIP);

String infoType = "";
String data = "webrtc";
String answer = "";
int status = 0;
String key3 = "";
long mesage_number = 0;
long msg0 = 0;
long msg1 = 0;
long msg2 = 0;
long msg3 = 0;
long msg4 = 0;
long msg5 = 0;
long msg6 = 0; // TCP disconnections
long msg7 = 0; // TCP connections
long msg8 = 0; // TCP sentences
long msg9 = 0;
String msg10 = "not connected";
String msg11 = "0";
String msg12 = "0";
String msg13 = "0";
String msg14 = "0";
/***************************************************/

void setup() {
  Serial.begin(57600);
	delay(2000);

	rmdevice.setMAC(devicemac);
	for (int i = 0; i < 6; i++) {
		Serial.print(devicemac[i],HEX);
		if(i<5) Serial.print(".");
	}
	Serial.println();
	
	Serial.println("Ethernet using DHCP");
	if (Ethernet.begin(devicemac) == 0) {
		Serial.println("DHCP Failed");
		Serial.println("Device STOP");
		for(;;)
		  ;
	}

	for (int i = 0; i < 4; i++) {
		Serial.print(Ethernet.localIP()[i], DEC);
		if(i<3) Serial.print(".");
	}
	Serial.println();
	
	// G E T   C H A N N E L   L O G I N   L I N K
	Serial.println("G E T   C H A N N E L   L O G I N   L I N K");
	rmdevice.info( infoType="datachannel", data="LiveData", answer );
	infoEvaluate( infoType, answer );


	Serial.println("0...LiveTCP_init");
	key3 = answer;
	status = 1; // we want to connect or to keep connected
	rmdevice.LiveTCP_init( key3, status ); // connect to the server data channel
	answer = String(status);
	infoEvaluate( infoType, answer );

	Serial.println("End Setup");
}

void loop()
{
/*
	rmdevice.info( infoType="online", data, answer );
	infoEvaluate( infoType, answer );

	rmdevice.info( infoType="server", data, answer );
	infoEvaluate( infoType, answer);

	rmdevice.info( infoType="getprofile", data, answer );
	infoEvaluate(infoType, answer );
	jsonExtract(answer, "id");
	jsonExtract(answer, "name");
	jsonExtract(answer, "datachannels");

	mesage_number++;
	data = "{%22nazdar%22:" + String(mesage_number) + "}";
	rmdevice.info( infoType="message", data, answer );
	infoEvaluate(infoType, answer );
	
	rmdevice.info( infoType="setiptcp", data="19.85.74.86", answer );
	infoEvaluate(infoType, answer );
	
	rmdevice.info( infoType="setporttcp", data="19006", answer );
	infoEvaluate(infoType, answer );
	
	rmdevice.info( infoType="setipudp", data="19.85.74.86", answer );
	infoEvaluate(infoType, answer );
	
	rmdevice.info( infoType="setportudp", data="19007", answer );
	infoEvaluate(infoType, answer );
	
	rmdevice.info( infoType="tablereadkey", data="webrtc", answer );
	infoEvaluate( infoType, answer );
	
	String key1 = answer;
	rmdevice.dataTableRead( key1, data, answer );
	infoEvaluate( infoType, answer );
	
	rmdevice.info( infoType="tablewritekey", data="webrtc", answer );
	infoEvaluate( infoType, answer );

	String key2 = answer;
	String _data = "";
	makeDataTableData(_data);
	rmdevice.dataTableWrite( key2, _data, answer );
	infoEvaluate( infoType, answer );
*/	
	Serial.println("1...LiveTCP_init");
	status = 1; // we want to connect or to keep connected
	
	// C O N N E C T   A N D   L O G I N   T O   T H E   S E R V E R
	rmdevice.LiveTCP_init( key3, status ); // connect to the server data channel
	// STATUS = 1 CONNECTED AND LOGIN SENT
	answer = String(status);
	Serial.println("1...evaluate start");
	infoEvaluate( infoType, answer );
	Serial.println("1...evaluate end");
	Serial.println("1...END");
	Serial.println();
	Serial.println();
	// READ SERVER ANSWER
	Serial.println("2...LiveTCP_read");
	rmdevice.LiveTCP_read( answer, status );  // read data from data channel
	

	if( status == 6 ){
		// A N A L Y S E  D A T A   F R O M   T H E   S E R V E R
		Serial.println("2...evaluate start");		
		infoEvaluate( infoType, answer );
		Serial.println("2...evaluate end");
	}
	Serial.println("2...END");
	Serial.println();
	Serial.println();

	// V E R I F Y   L O G I N   T O   T H E   S E R V E R
	status = 1;
	Serial.println("3...LiveTCP_init");
	rmdevice.LiveTCP_init( key3, status ); // VERIFY CONNECTION
	Serial.println("3...evaluate start");
	infoEvaluate( infoType, answer );
	Serial.println("3...evaluate end");
	Serial.println("3...END");
	Serial.println();
	Serial.println();
	
	if( status == 1 ){
		Serial.println("4...LiveTCP_write");	
		String datatosend = String(millis());
		// S E N D   D A T A   T O   T H E   S E R V E R
		rmdevice.LiveTCP_write( datatosend, status ); // write data to data channel
		Serial.println("4...evaluate start");	
		infoEvaluate( infoType = "datachannel", String(status) );
		Serial.println("4...evaluate end");
	}
	Serial.println("4...END");
	Serial.println();
	Serial.println();
/*
	Serial.println("TCP REDING ... ");
	rmdevice.LiveTCP_read( data, status );  // read data from data channel
	answer = String(status);
	infoEvaluate( infoType, answer );

	Serial.println("TCP WRITING ... ");	
	String datatosend = String(millis());
	rmdevice.LiveTCP_write( datatosend, status ); // write data to data channel
	answer = String(status);
	infoEvaluate( infoType, answer );
*/
}

void infoEvaluate(String _infoType, String _data ){
	Serial.println(_infoType);
	Serial.print("_data.length= ");
	Serial.println(_data.length());
	Serial.println(_data);
	// returned http data: 0=failed, 1=sent, 2=waiting, 3=done, 4=timeout, string for data
	// returned TCP data: 0, 0:0, X:X, XX:XX
	// I N T E R N A L   T C P   S T A T U S
	// LiveTCP_init - 0 not connected / 1 connected
	// LiveTCP_write   - 0 not connected/not sent => try 3x LiveTCP_init and resend / 4 writing complete
	// LiveTCP_read    - 5 reading / 6 reading complete / 10 no listenner / 11 one listenner (32 = 22 listenners)
	
	// H T T P
	if( _data.length() <= 1 && _infoType != "datachannel" ) {
		Serial.print(_infoType);
		Serial.print(":");
		Serial.print(_data);
		if( _data == "0" ){
			Serial.println("failed");
			msg0++;
		} else if( _data == "1" ){
			Serial.println("sent");
			msg1++;
		} else if( _data == "2" ){
			Serial.println("waiting");
			msg2++;
		} else if( _data == "3" ){
			Serial.println("done");
			msg3++;
		} else if( _data == "4" ){
			Serial.println("timeout");
			msg4++;
		} else Serial.println("data != 01234");
		
	} else {
		msg5++; //count strings recieved
		Serial.print(_infoType);
		Serial.print(":");
		Serial.println(_data);
	}

	// T C P
	if( _infoType == "datachannel" ) {
		if(_data == "0"){
			Serial.println("TCP IS DISCONNECTED");
			msg6++;
			msg10 = "not connected";
		} else if(_data == "1"){
			Serial.println("TCP SUCCESSFULY CONNECTED");
			msg7++;
			msg10 = "connected";
		} else if(_data == "2"){
			// free
		} else if(_data == "3"){
			// free
		} else if(_data == "4"){
			msg11 = "TCP writing OK";
		} else if(_data == "5"){
			msg12 = "TCP reading ...";
		} else if(_data == "6"){
			msg12 = "TCP reading OK";
		} else if(_data == "10"){
			msg13 = "no listenners";
		} else if(_data[0] == '1' ){
			msg13 = ">1 listenners";
		} else if(_data[0] == '2' ){
			msg13 = "<10 listenners";
		} else if(_data[0] == '3' ){
			msg13 = "<20 listenners";
		} else if(_data.length() >= 1 ){
			msg8++;
		}
	}

	for(int i=0;i<20;i++) Serial.print("-");
	Serial.println();
	Serial.println("--- H T T P ---");
	Serial.print("failed  = "); Serial.println(msg0);
	Serial.print("sent    = "); Serial.println(msg1);
	Serial.print("waiting = "); Serial.println(msg2);
	Serial.print("done    = "); Serial.println(msg3);
	Serial.print("timeout = "); Serial.println(msg4);
	Serial.print("strings = "); Serial.println(msg5);
	Serial.println("--- T C P ---");
	Serial.print("TCP = "); Serial.println(msg10);
	Serial.print("nb dis = "); Serial.println(msg6);
	Serial.print("nb con = "); Serial.println(msg7);
	Serial.print("nb sen = "); Serial.println(msg8);
	Serial.print("writ = "); Serial.println(msg11);
	Serial.print("read = "); Serial.println(msg12);
	Serial.print("user = "); Serial.println(msg13);
	Serial.println("500 ms delay...");
	for(int i=0;i<20;i++) Serial.print("-");

	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	delay(500);
}

void jsonExtract(String _str1, String _str2){
	if (_str2.equals("id")) {
		int firstListItem = _str1.indexOf("id"); // "id":"number"
		int secondListItem = _str1.indexOf(44, firstListItem); // char 34 = "
		String msg = _str1.substring(firstListItem+4,secondListItem);
		Serial.println();
		Serial.print("msg_ID = ");
		Serial.println(msg);
   } else if(_str2.equals("name")){
		int firstListItem = _str1.indexOf("name"); // "name":"stringname"
		int secondListItem = _str1.indexOf(34, firstListItem + 7); // char 34 = "
		String msg = _str1.substring(firstListItem+7,secondListItem);
		Serial.println();
		Serial.print("msg_name = ");
		Serial.println(msg);   	
   } else if(_str2.equals("datachannels")){
		int firstListItem = _str1.indexOf("datachannels"); // "datachannels"=["60_LiveData","60_LiveData2","60_LiveData3"]
		int secondListItem = _str1.indexOf(34, firstListItem + 16); // char 34=" 91=[ 93=]
		String msg1 = _str1.substring(firstListItem+16, secondListItem);

		firstListItem = _str1.indexOf(34, secondListItem+1); // "datachannels":"stringname"
		secondListItem = _str1.indexOf(34, firstListItem + 1); // char 44=,
		String msg2 = _str1.substring(firstListItem+1,secondListItem);

		firstListItem = _str1.indexOf(34, secondListItem+1); // "datachannels":"stringname"
		secondListItem = _str1.indexOf(34, firstListItem + 1); // char 44=,
		String msg3 = _str1.substring(firstListItem+1,secondListItem);

		firstListItem = _str1.indexOf(34, secondListItem+1); // "datachannels":"stringname"
		secondListItem = _str1.indexOf(34, firstListItem + 1); // char 44=,
		String msg4 = _str1.substring(firstListItem+1,secondListItem);

		Serial.println();
		Serial.println("msg_datachannels = ");
		Serial.println(msg1);
		Serial.println(msg2);
		Serial.println(msg3);
		Serial.println(msg4);
		
		}
}

void makeDataTableData(String& _str){

	String _mac = "";
	for (int i = 0; i < 6; i++) {
	_mac += String(devicemac[i],HEX);
	//Serial.println(_mac);
		if(i<5) _mac += ".";
	}
	//Serial.println(_mac);
	String _ip = "";
	for (int i = 0; i < 4; i++) {
		_ip += String(Ethernet.localIP()[i], DEC);
		//Serial.println(_ip);
		if(i<3) _ip += ".";
	}
	//Serial.println(_ip);
	String _port = String(portHTTP);
	//Serial.println(_port);
	_str = "{%22MAC%22:%22" + _mac +"%22,%22IP%22:%22" + _ip +"%22,%22PORT%22:%22" + _port +"%22,%22link%22:%22http://something%22,%22size%22:{%22height%22:240,%22width%22:320},%22lastTimestamp%22:12547,%22framerate%22:25,%22audio%22:1}";
	//Serial.println(_str);
}




















