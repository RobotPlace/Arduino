#include <SoftwareSerial.h>
#define rxPin 6
#define txPin 7

#include <SPI.h>
#include <Ethernet.h>

//*****************************
// robotMonitor SETUP
// NEED TO BE CHANGED IF NEEDED
byte mac[] = { 0xAA, 0xAB, 0xBC, 0xCF, 0xDE, 0xED };
IPAddress robotIP = (192,168,1,160);
unsigned int portTCP = 50160; //unsigned int varxyz = max 65,535

// Initialize the Ethernet client that connect to robotmonitor.com:
IPAddress server(46,28,111,207); // portal.robotmonitor.com server
EthernetClient clientPING;
EthernetClient clientDATA;

// >>> DEVICE TO MODIFY
String robotName = "#58";
String sourceFile = "i141106...ID58"; //"i141106_Mesh_server_DataTable_ping_ID59";
String DeviceAuthKey = "cfc204db7043a5bf92d603b1a24ff127";
String tableinLink = "54e52e6af36a26878cef6ab8921561a5";
// <<< DEVICE TO MODIFY

int sensorID = 0;
//DataTable
//http://portal.robotmonitor.com/device/in/a00c2109d1302ff2aa1fcc9b65abbf78/?data={"nameValArray":[5,6,4]}
//data structure {"Temp_RH":[Temp,RH]}
//to store incoming data dfrom other sensors by XBee
String bufferSerial = "";
String bufferSWSerial = "";
//is there a sensor connected to this board?

boolean clientDATAConection = false;
String bufferData = "";
boolean dataStart = false;
boolean retry = false;
String retryStr = "";
//String pingProcessInfo = "ping";
unsigned long pingperiod = 90000;
unsigned long lastping = 0;
boolean clientPINGConection = false;
boolean pingDone = false;
String bufferPING = "";
unsigned long pingVoidTime = 0;
unsigned long pingVoidTimeLast = 0;
unsigned long dataVoidTime = 0;
unsigned long dataVoidTimeLast = 0;
String XBeeID = "";
SoftwareSerial mySerial(rxPin, txPin); // RX, TX
int DTerrors = 0;
int PGerrors = 0;
