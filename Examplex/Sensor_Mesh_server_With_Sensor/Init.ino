#include <SoftwareSerial.h>
#define rxPin 6
#define txPin 7

//DHT-sensor-library
#include <DHT.h> // download at https://github.com/adafruit/DHT-sensor-library

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

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

// device
String robotName = "#59";
String sourceFile = "i141106...ID59"; //"i141106_Mesh_server_DataTable_ping_ID59";
String KEY = "d8e87f5399ca55f862dcb0b290b84f54";
int sensorID = 0;
//DataTable
//http://portal.robotmonitor.com/device/in/a00c2109d1302ff2aa1fcc9b65abbf78/?data={"nameValArray":[5,6,4]}
//data structure {"Temp_RH":[Temp,RH]}
String LINK = "7d197111e3de218ff2dd942416ae9309";
//to store incoming data dfrom other sensors by XBee
String bufferSerial = "";
String bufferSWSerial = "";
//is there a sensor connected to this board?
boolean onboardSensor = false;

//String dataProcessInfo = "data";
unsigned long dataReadPeriod = 1000;
unsigned long dataSendPeriod = 600000;
unsigned long lastdataRead = 0;
unsigned long lastdataSend = 0;
boolean clientDATAConection = false;
String bufferData = "";
int hLast = 0;
int tLast = 0;
int h = 0;
int t = 0;
boolean dataStart = false;

//String pingProcessInfo = "ping";
unsigned long pingperiod = 110000;
unsigned long lastping = 0;
boolean clientPINGConection = false;
boolean pingDone = false;
String bufferPING = "";

String XBeeID = "";
SoftwareSerial mySerial(rxPin, txPin); // RX, TX
int DTerrors = 0;
int PGerrors = 0;
