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

 RobotMonitor HTTP CALLS
	send command example:
		LAN: http://192.168.1.180:50180/a
		WAN: http://yourDomain:50180/a
	send pingonline status example:
		http://portal.robotmonitor.com/device-acces/write/?hash=yourDeviceAuthHashKey&data=pingonline

 Circuit:
  - Arduino
  - Arduino Ethernet shield
  - LED on pin 7
 */
 
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xEE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress robotIP(192,168,1,184);
int portHTTP = 51840;

//Device info - useful when need to know what code is uploaded
String robotName = "Code: Demo_04_HTTP_LED_simplest";
String robotOwner = "Petr Hubacek";

//Initialize the Ethernet server library
EthernetServer serverHTTP(portHTTP);          // local port to listen on
String buffer = "";
int pin_led = 7;
int ledState = LOW;
String lastCMD = "s";
float lastUserAction = 0;

void setup()
{
  // start the Ethernet connection:
  Ethernet.begin(mac, robotIP);
  // give the Ethernet shield a second to initialize:
  delay(500);
  serverHTTP.begin();
  // initialize the digital pin as an output.
  pinMode(7, OUTPUT);
  Serial.begin(57600);
  Serial.print("robotName = ");
  Serial.println(robotName);
  Serial.print("LED Pin = ");
  Serial.println(pin_led);
}

void loop()
{
  clientHTTP();
  heartbeat_check();   // must stay in loop() to insure propes timing check not depending on other functions
}

//Ensures the LED in ON only for 5 seconds
void heartbeat_check(){
  if(millis() - lastUserAction > 5000){
   led_blink(0);
  }
}

void clientHTTP()
{
  // listen for incoming clients
  EthernetClient client = serverHTTP.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        buffer += c;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          String command = buffer.substring(5,6);
          Serial.println(command);
          buffer = "";
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          if (command == "A" || command == "a") {
            led_blink(1);
            client.print("LED ON!");
            client.println("<br />");
          } 
          else if (command == "S" || command == "s") {
            led_blink(0);
            client.print("LED OFF");
            client.println("<br />");
          } 
          else {
            client.print("Device answer: Command unknown");
            client.println("<br />");
            client.println("robotName: " + robotName + "<br />");
            client.println("HTTP commands: ON:A,a; OFF:S,s <br />");
          }
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
    delay(2);
    // close the connection:
    client.stop();
  }
}

void led_blink(int _cmd) {
  //turns ON/OFF the LED
  if (_cmd == 1) ledState = HIGH;
  else ledState = LOW;
  digitalWrite(pin_led, ledState);
  lastUserAction = millis();
}

