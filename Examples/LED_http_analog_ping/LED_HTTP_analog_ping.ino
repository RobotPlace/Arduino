/*
 Based on Arduino Web Server Exemple
 created 20.Sept 2013
 by Petr Hubacek

 SKETCH PRINCIPLE:
 A simple web server to turn a LED ON/OFF by HTTP requests from robotmonitor.com Monitors.
 - HTTP string answer to string recieved by HTTP IP + Port
 - LED ON/OFF according recieved values
 - user activity check to avoid LED switched on for longer time than ie. 5 seconds

 COMMANDS:
  - LED ON command:  A or a
  - LED OFF command: S or s

 Circuit:
  - Arduino
  - Arduino Ethernet shield
  - LED on pin 7

 This sketch is released under MIT licence
*/

#include <SPI.h>
#include <Ethernet.h>

/****** TO MODIFY ***************/
byte mac[] = { 0xDE, 0x82, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress robotIP(192,168,1,183);
unsigned int portHTTP = 51830;
String authHash = "enterYourDeviceAuthKey";
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

String buffer = "";
int ledStateA = 0;
int ledPin = 9;

float lastLED = 0;
int resetLED = 3000;

void loop()
{
  clientHTTP();
  led_reset();
  ping();
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  // start the Ethernet connection:
  Ethernet.begin(mac, robotIP);
  // give the Ethernet shield a second to initialize:
  serverHTTP.begin();
  delay(100);

  pinMode(ledPin, OUTPUT);

  Serial.print("robotName: ");
  Serial.println(robotName);
  Serial.print("sourceFile: ");
  Serial.println(sourceFile);
  Serial.print("robotIP: ");
  for(int i=0; i<=3;i++){
    Serial.print(robotIP[i], DEC);
    if(i < 3) Serial.print(".");
  }
  Serial.println();
  Serial.print("portHTTP: ");
  Serial.println(portHTTP);
  Serial.print("LED Pin: ");
  Serial.println(ledPin);
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
            ledBlink(10);
            client.print("LED 5%");
            client.println("<br />");
          }
          else if (command == "W" || command == "w") {
            Serial.println(command);
            ledBlink(50);
            client.print("LED 20%");
            client.println("<br />");
          }
          else if (command == "E" || command == "e") {
            Serial.println(command);
            ledBlink(100);
            client.print("LED 60%");
            client.println("<br />");
          }
          else if (command == "A" || command == "a") {
            Serial.println(command);
            ledBlink(255);
            client.print("LED 100%");
            client.println("<br />");
          }
          else if (command == "S" || command == "s") {
            Serial.println(command);
            ledBlink(0);
            client.print("LED OFF");
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

void ledBlink(int _switch) {
  
  analogWrite(ledPin, _switch);
  lastLED = millis();
  
}

void led_reset() {
  if(millis() - lastLED > resetLED){
    ledBlink(0);
  }
}

unsigned long pingperiod = 90000;
unsigned long lastping = 0;

void ping()
{
  // if you get a connection, report back via serial:
  if (millis() - lastping >= pingperiod) {
    if (client.connect(server, 80)) {
      Serial.println("connected");
      client.println("GET /device-acces/write/?hash=" + authHash + "&data=pingonline HTTP/1.0");
      client.println("Host: portal.robotmonitor.com");
      client.println("Connection: close");
      client.println();
      Serial.println("ping OK");
      client.stop();
      lastping = millis();
    }
    else {
      // if you didn't get a connection to the server:
      Serial.println("ping failed");
    }
  }
}
