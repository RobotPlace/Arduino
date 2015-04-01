/*
 This sketch is released under MIT licence

 created 21 Mar 2015
 by Petr Hubacek
 
 LED_TCOP_simplest.pde:
 This sketch:
 1) login to LiveData robotmonitor.com server with TCP packet
 2) wait for user commands, server answers or server error messages
 3) switch one LED ON/OFF according command

*/

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <RobotMonitor.h>

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

//for ping, read details and dataTables
RobotMonitor rmdevice("deviceKey");

EthernetClient client;

String bufferTCP = "";
boolean bufferTCPcomplete = false;
String bufferSerial = "";
String ReplyBuffer = "";

//needed for LED process
int ledPin = 9;
int ledState = 0;
unsigned long lastLED = 0;
int resetLEDtime = 3000;
unsigned long lastHeartbeat = 0;

unsigned long lastConnected = 0;
String buffer = "";
int listeners = 0;
int connRefreshPeriod = 2000;
boolean logged = false;
int permanentError = 0;

void setup() {
  Serial.begin(57600);
  Serial.println(F("setup START"));
  Serial.println(F("www.robotmonitor.com"));
  pinMode(ledPin, OUTPUT);
  
  // start the Ethernet DHCP:
  Serial.println("DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to DHCP IP address"));
    // initialize the ethernet device not using DHCP:
    Ethernet.begin(mac, robotIP);
  } 
  else {
    robotIP = Ethernet.localIP();
  }

  Serial.print(F("robotIP = "));
  for (int i = 0; i <= 3; i++) {
    Serial.print(robotIP[i], DEC);
    if (i < 3) Serial.print(".");
  }
  
  Serial.println();
  Serial.print(F("localport = "));
  Serial.println(localPort);
  Serial.print(F("serverTCP = "));
  Serial.println(serverTCP);
  Serial.print(F("portTCP = "));
  Serial.println(portTCP);
  Serial.print(F("robotName: "));
  Serial.println(robotName);
  Serial.print(F("sourceFile: "));
  Serial.println(sourceFile);
  Serial.print(F("LiveData login: "));
  Serial.println(loginStr);
  connectTCP();
  Serial.println(F("*******************************"));

  Serial.println("setup END");
}

void loop()
{
  readTCP();
  connRefresh();
  led_reset();
  heartbeat();
  rmdevice.ping();
}

//NO CHANGE NEEDED FROM USER
void sendTCP(String _str)
{
  Serial.print(".");
  if (client.connected()){
    client.print(_str);
    client.print('\r\n');
    Serial.println("/sent");
    }  // if the server's disconnected, stop the client:
  else if (!client.connected()){
      Serial.println(F("disconnected"));
      client.stop();
      connectTCP();
    }
  lastConnected = millis();
}

//NO CHANGE NEEDED FROM USER
void connectTCP()
{
  Serial.print(",");
  // if you get a connection, report back via serial:
  if (client.connect(serverTCP, 8080)) {
    Serial.println(F("reConnectTCP():"));
    Serial.println(loginStr);
    client.println(loginStr);
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println(F("connection failed"));
  }
}

void sendTCP_echo(String _str) {
  // send a reply, to the IP address and port that sent us the packet we received
  Serial.print(".");
  if (client.connected()){
  	client.print(ReplyBuffer);
    client.print(_str);
    client.print('\r\n');
    Serial.println("/sent");
  }  // if the server's disconnected, stop the client:
  else if (!client.connected()){
      Serial.println(F("disconnected"));
      client.stop();
      connectTCP();
    }
  lastConnected = millis();
}

void readTCP(){
  // if there are incoming bytes available 
  // from the server, read them and print them to robots:
  if(client.available()){
    char c = client.read();
    bufferTCP += c;
    //Serial.print(c);
    //check if there are listeners
    if(bufferTCP.length() == 1){
      if (c >= 48 || c <= 57){
        listeners = int(c) - 48;
        if(listeners > 0 && listeners <= 9){
          Serial.println(F("listeners > 0"));
          connRefreshPeriod = 5000;
        }
        else if(listeners = 0){
          Serial.println(F("listeners = 0"));
          connRefreshPeriod = 2000;
        }
      }
    }
    bufferTCPcomplete = true;
    lastConnected = millis();
  }
  else {
    if(bufferTCPcomplete){
      //Serial.print(bufferTCP);
      processData(bufferTCP);
      bufferTCP = "";
      bufferTCPcomplete = false;
    }
  }
}

//NO CHANGE NEEDED FROM USER
//heartbeat() is optional. It solves server disconnection if no data are sent for more than 10 minutes
void connRefresh(){
  if(millis()-lastConnected > connRefreshPeriod){
    Serial.println(F("REFRESH"));
    //send or reconnect
    sendTCP("@");
    sendTCP((String)lastConnected);
    sendTCP("\r\n");
    Serial.print("@");
    Serial.print((String)lastConnected);
    Serial.println();
  }
}


void processData(String _str) {

  //parse the message to commands and to switch values
  int switch_value = -1;
  if (_str[0] == 'S' || _str[0] == 's') switch_value = 0;
  else if (_str[0] == 'A' || _str[0] == 'a') switch_value = 1;
  else if (_str[0] >= 48 && _str[0] <= 57) switch_value = 2; //we have got number of users online
  else if (_str[0] == 'E') switch_value = 3; //E (missing parameter)
  if (switch_value < 3) permanentError = 0;

  switch (switch_value) {
    case 0:
      Serial.println(F("=>S")); // LED ON
      ledState = 0;
      ledBlink(ledState);
      break;
    case 1:
      Serial.println(F("=>A")); // LED OFF
      ledState = 250;
      ledBlink(ledState);
      break;
    case 2:
      Serial.println(F("=> users online"));
      logged = true;
      break;
    case 3:
      Serial.println(F("=> Login error"));
      permanentError++;
      Serial.print(F("=> permanentError = "));
      Serial.println(permanentError);
      delay(1000);
      sendTCP("1");
      if ( permanentError > 5 ) {
        Serial.println(F("permanentError > 5 => 1 minute ON HOLD"));
        permanentError = 0;
        delay(60000);
      }
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      delay(10);
  }
  delay(10);
}

void ledBlink(int _switch) {
  analogWrite(ledPin, _switch); //PWM transforms 5V (255) to 3V (153)
  lastLED = millis();
}

void led_reset() {
  if ( (millis() - lastLED > resetLEDtime) && ledState != 0 ) {
    ledState = 0;
    ledBlink(ledState);
  }
}

// heartbeat() is optional. It solves server disconnection if no data are sent for more than 10 minutes
void heartbeat() {
  if (millis() - lastHeartbeat > 30000) {
    String timestamp = String(millis()/1000);
    sendTCP(timestamp);
    Serial.println(timestamp);
    lastHeartbeat = millis();
  }
}
