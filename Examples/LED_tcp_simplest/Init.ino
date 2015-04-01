#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>

/****** TO MODIFY ***************/
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress robotIP(192, 168, 1, 187);
unsigned int localPort = 51840;      // local port to listen on
String robotName = "EX3_TCP_LED_US2";
String sourceFile = "Demo_LED_TCP_simplest";
String authHash = "7ce6eb6a8e382cc6eb00d83efd8c1b0a";
//IPAddress server(192, 168, 1, 13);
char serverTCP[] = "us2.robotmonitor.com"; // time.nist.gov NTP server
int portTCP = 8080;
char loginStr[] = "devid=73&chname=73_LiveData&chhash=8abd94d60e49f4205eb92b7dbb737d8f";
/****** TO MODIFY ***************/

EthernetClient client;

String bufferTCP = "";
boolean bufferTCPcomplete = false;
String bufferSerial = "";

//needed for LED process
int ledPin = 9;
int ledState = 0;
unsigned long lastLED = 0;
int resetLEDtime = 3000;

unsigned long lastConnected = 0;
String buffer = "";
int listeners = 0;
int connRefreshPeriod = 2000;
boolean logged = false;
int permanentError = 0;
