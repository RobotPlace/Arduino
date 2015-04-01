/*
 This sketch is released under MIT licence
 Based on Arduino Web Server Exemple
 created 20.Sept 2013
 by Petr Hubacek

 SKETCH PRINCIPLE:
 A simple web server to change the light strength of a LED by HTTP request commands.
 - HTTP string answer to string recieved by HTTP IP + Port
 - 4 LED ON/OFF according recieved values
 - user activity check to avoid LED switched on for longer time than ie. 5 seconds

 COMMANDS:
	This will switch your LED ON/OFF by commands:
	q = LED 1 ON
	w = LED 2 ON
	e = LED 3 ON
	a = LED 4 ON
	s = All LEDs OFF

 Circuit:
  - Arduino
  - Arduino Ethernet shield
  - LED on pin 9
*/

#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <TrueRandom.h> //randomly generated MAC address

// --------------- TrueRandom sets this, so no default is needed here.
byte mac[6];

/****** TO MODIFY ***************/
IPAddress robotIP(192,168,1,189);
unsigned int portHTTP = 51890;
String authHash = "xxxx";
//RobotMonitor SETUP
String robotName = "HTTP_LED analogWrite";
String sourceFile = "Demo_LED_HTTP_analog";
String robotType = "Demo";
String robotOwner = "your name";
/****** TO MODIFY ***************/

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
IPAddress server(46,28,111,207); // www.robotmonitor.com 

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
EthernetServer serverHTTP(portHTTP);          // local port to listen on

unsigned long pingperiod = 5000;
unsigned long lastping = 0;
boolean clientConection = false;
boolean clientAnswered = false;

String buffer = "";
int pinLed[] = {9,6,5,3};	// lets use pins with PWM possibility with the ATmega168 or ATmega328

float lastLED = 0;
int resetLED = 3000;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  // Set a random MAC address using TrueRandom
  // and Read/Store MAC address in EEPROM
  //EEPROM.write(7, 99);  //uncomment to change MAC address
  if (EEPROM.read(7) == '#') {
    for (int i = 0; i < 6; i++) {
      mac[i] = EEPROM.read(i);
      Serial.print(mac[i],HEX);
		Serial.print(".");
    }
  } else {
    for (int i = 0; i < 6; i++) {
      mac[i] = TrueRandom.randomByte();
      EEPROM.write(i, mac[i]);
    }
    EEPROM.write(7, '#');
  }
	Serial.println();

  // start the Ethernet connection:
  Ethernet.begin(mac, robotIP);
  // give the Ethernet shield a second to initialize:
  serverHTTP.begin();
  delay(100);

  pinMode(pinLed[0], OUTPUT);
  pinMode(pinLed[1], OUTPUT);
  pinMode(pinLed[2], OUTPUT);
  pinMode(pinLed[3], OUTPUT);
  Serial.print(F("robotName: "));
  Serial.println(robotName);
  Serial.print(F("sourceFile: "));
  Serial.println(sourceFile);
  Serial.print(F("robotIP: "));
  for(int i=0; i<=3;i++){
    Serial.print(robotIP[i], DEC);
    if(i < 3) Serial.print(".");
  }
  Serial.println();
  Serial.print(F("portHTTP: "));
  Serial.println(portHTTP);
  Serial.print(F("LED Pin: "));
  Serial.println(pinLed[0]);
  Serial.print(F(", "));
  Serial.println(pinLed[1]);
  Serial.print(F(", "));
  Serial.println(pinLed[2]);
  Serial.print(F(", "));
  Serial.println(pinLed[3]);
}

void loop()
{
  clientHTTP();
  led_reset();
  ping();
}

void clientHTTP(){
  // listen for incoming clients
  //arduino1.0.1:  EthernetClient client = serverHTTP.available();
  //arduino22:  
  buffer = "";
  EthernetClient client = serverHTTP.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        //Serial.println("available");
        char c = client.read();
        buffer += c;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' ) { //&& currentLineIsBlank
          String command = buffer.substring(5,6);
          Serial.print("command = ");
          //Serial.print(command);
          buffer = "";
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          if (command == "Q" || command == "q") {
            Serial.println(command);
            ledBlink(0, 200);
            client.print("LED 1 ON");
            client.println("<br />");
          }
          else if (command == "W" || command == "w") {
            Serial.println(command);
            ledBlink(1, 200);
            client.print("LED 2 ON");
            client.println("<br />");
          }
          else if (command == "E" || command == "e") {
            Serial.println(command);
            ledBlink(2, 200);
            client.print("LED 3 ON");
            client.println("<br />");
          }
          else if (command == "A" || command == "a") {
            Serial.println(command);
            ledBlink(3, 200);
            client.print("LED 4 ON");
            client.println("<br />");
          }
          else if (command == "S" || command == "s") {
            Serial.println(command);
            ledBlink(0,0);
            ledBlink(1,0);
            ledBlink(2,0);
            ledBlink(3,0);
            client.print("ALL LEDs OFF");
            client.println("<br />");
          }
          else if (command == "f" || command == "F") {
            // ignoring favicon
          }
          else {
            client.print("Device answer: Command unknown");
            client.println("<br />");
            client.println("robotName: " + robotName + "<br />");
            client.println("HTTP commands: q,w,e,a,s <br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }

      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}

void ledBlink(int _pin, int _switch) {
  
  analogWrite(pinLed[_pin], _switch);
  lastLED = millis();
  
}

void led_reset() {
  if(millis() - lastLED > resetLED){
    ledBlink(0,0);
    ledBlink(1,0);
    ledBlink(2,0);
    ledBlink(3,0);
  }
}

void ping()
{
  // if you get a connection, report back via serial:
  if (millis() - lastping >= pingperiod) {
    if (client.connect(server, 80)) {
      clientConection = true;
      clientAnswered = false;
      pingperiod = 60000;
      Serial.println("connected");
      client.println("GET /device-acces/write/?hash=" + authHash + "&data=pingonline HTTP/1.0");
      client.println("Host: portal.robotmonitor.com");
      client.println("Connection: close");
      client.println();
      Serial.println("ping OK");
    }
    else {
      // if you didn't get a connection to the server:
      clientConection = false;
      pingperiod = 600000;
      Serial.println(F("ping failed"));
    }
    lastping = millis();
   }

	if (client.available()) {
		while (client.available()){
			//Serial.println("available");
			char c = client.read();
			//Serial.print(c);
			  if (c == 10){
			  	c = client.read();
				  if (c == '1'){
				  	Serial.println(c);
				  	Serial.println(millis()-lastping);
				  	Serial.println("PING DONE");
				  	clientAnswered = true;
				  }
			  }
		}
	}
	
	// if the server's disconnected, stop the client:
	if (clientConection == true && clientAnswered == true) { //!client.connected() && 
		client.stop();
		clientConection = false;
		Serial.println(F("disconnected"));
	}
}







