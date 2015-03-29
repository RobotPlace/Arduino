/*
  source: https://github.com/ejeklint/ArduinoWebsocketServer/edit/master/examples/Websocket_Demo/Websocket_Demo.ino

  modified on Mars 2015
	by Petr Hubacek
*/
#include <SPI.h>
#include <Ethernet.h>

// Enabe debug tracing to Serial port.
#define DEBUG

// Here we define a maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 64

#include "WebSocket.h"

byte mac[] = { 0x52, 0x4F, 0x43, 0x4B, 0x45, 0x54 };
byte ip[] = { 192, 168, 1 , 184 };

// Create a Websocket server
WebSocketServer wsServer;

int pinLed = 9; // pins 10,11,12,13 are used by ethernet shield!!
int ledState = 0;
unsigned long lastUserAction = 0;
int pingWSperiod = 1000;
unsigned long lastpingWS = 0;
String bufferData = "";

void onConnect(WebSocket &socket) {
  Serial.println("onConnect called");
}


// You must have at least one function with the following signature.
// It will be called by the server when a data frame is received.
void onData(WebSocket &socket, char* dataString, byte frameLength) {
  
#ifdef DEBUG
  Serial.print("Got data: ");
  Serial.write((unsigned char*)dataString, frameLength);
  Serial.println();
#endif
  
  // Just echo back data for fun.
  socket.send(dataString, strlen(dataString));
  bufferData = dataString;
}

void onDisconnect(WebSocket &socket) {
  Serial.println("onDisconnect called");
}

void setup() {
#ifdef DEBUG  
  Serial.begin(57600);
#endif
  Ethernet.begin(mac, ip);
  
  wsServer.registerConnectCallback(&onConnect);
  wsServer.registerDataCallback(&onData);
  wsServer.registerDisconnectCallback(&onDisconnect);  
  wsServer.begin();
  
  pinMode(pinLed, OUTPUT);
  delay(100); // Give Ethernet time to get ready
}

void loop() {
  // Should be called for each loop.
  wsServer.listen();
  heartbeat_check();
  if(bufferData.length() > 0) processCMD(bufferData);
  
  delay(10); // make some dela. If "zero", ws dosconnects if something received.
  
  if (wsServer.connectionCount() > 0) {
  	if(millis() - lastpingWS > pingWSperiod){
    	wsServer.send("1", 1);
    	lastpingWS = millis();
    	Serial.print("ping ");
    	Serial.println(lastpingWS);
  	}
  }
}

void processCMD(String _str) {
  Serial.println(_str.length());
  if (_str == "A" || _str == "a") {
    Serial.println("LED ON");
    led_blink(1);
  }
  else if (_str == "S" || _str == "s") {
    Serial.println("LED OFF");
    led_blink(0);
  }
  else if (_str == "R" || _str == "r") {
    Serial.println("LED TOGGLE");
    if(ledState == 0) led_blink(1);
    else if(ledState == 1) led_blink(0);
  }
  bufferData = "";
}

void led_blink(int _cmd) {
  //turns ON/OFF the LED
  ledState = _cmd;
  digitalWrite(pinLed, _cmd);
  lastUserAction = millis();
}

//Ensures the LED in ON only for 5 seconds
void heartbeat_check() {
  if (millis() - lastUserAction > 5000) {
    led_blink(0);
  }
}
