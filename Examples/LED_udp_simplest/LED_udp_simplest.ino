/*
 This sketch is released under MIT licence

 created 21 Mar 2015
 by Petr Hubacek
 
 LED_UDP_simplest.pde:
 This sketch:
 1) login to LiveData robotmonitor.com server with UDP packet
 2) wait for user commands, server answers or server error messages
 3) switch one LED ON/OFF according command

*/

#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

/****** TO MODIFY ***************/
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 187);
unsigned int localPort = 51870;      // local port to listen on
String robotName = "EX4_UDP_LED";
String robotOwner = "Petr Hubacek";
String authHash = "xxxxx";
IPAddress serverUDP(192, 168, 1, 13);
//char serverUDP[] = "us2.robotmonitor.com"; // time.nist.gov NTP server
int portUDP = 8080;
char loginStr[] = "devid=39&chname=39_LiveData&chhash=xxxxx";
/****** TO MODIFY ***************/

// buffers for receiving and sending data
const int maxSize = 128;
char packetBuffer[maxSize]; //buffer to hold incoming packet, //UDP_TX_PACKET_MAX_SIZE
char ReplyBuffer[] = "ECHO:";       // a string to send back
char heartbeatBuffer[] = "1";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

//needed for LED process
int ledPin = 9;
int ledState = 0;
unsigned long lastLED = 0;
int resetLED = 3000;
unsigned long lastHeartbeat = 0;
boolean logged = false;
int permanentError = 0;

void setup() {
  Serial.begin(57600);
  Serial.println("setup START");
  pinMode(ledPin, OUTPUT);
  // start the Ethernet and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);

  Serial.println(F("www.robotmonitor.com"));
  Serial.print(F("ip = "));
  for (int i = 0; i <= 3; i++) {
    Serial.print(ip[i], DEC);
    if (i < 3) Serial.print(".");
  }
  Serial.println();
  Serial.print(F("localport = "));
  Serial.println(localPort);

  Serial.print(F("serverUDP = "));
  Serial.println(serverUDP);
  Serial.print(F("portUDP = "));
  Serial.println(portUDP);
  Serial.print(F("robotName: "));
  Serial.println(robotName);
  Serial.println(F("*******************************"));
  Serial.print(F("LiveData login: "));
  Serial.println(loginStr);
  sendUDP_cmd(loginStr);
  Serial.println(F("*******************************"));

  Serial.println("setup END");
}

void loop() {
  processUDP();
  led_reset();
  heartbeat();
}

void processUDP() {
  // if there's data available, read a packet
  if ( Udp.parsePacket() > 0 ) {
    // read the packet into packetBufffer
    Udp.read(packetBuffer, maxSize); //UDP_TX_PACKET_MAX_SIZE
    Serial.print("Recieved:");
    Serial.println(sizeof(packetBuffer));
    Serial.print(packetBuffer);

    //parse the message to commands
    int switch_value = -1;
      if (packetBuffer[0] == 'S' || packetBuffer[0] == 's') switch_value = 0;
      else if (packetBuffer[0] == 'A' || packetBuffer[0] == 'a') switch_value = 1;
      else if (packetBuffer[0] >= 48 && packetBuffer[0] <= 57) switch_value = 2; //we have got number of users online  
      else if (packetBuffer[0] == 'E') switch_value = 3; //E (missing parameter)
      if(switch_value < 3) permanentError = 0;
      
      switch (switch_value) {
        case 0:
          Serial.println("=>S"); // LED ON
          ledState = 0;
          ledBlink(ledState);
          sendUDP_echo(packetBuffer);
          break;
        case 1:
          Serial.println("=>A"); // LED OFF
          ledState = 1;
          ledBlink(ledState);
          sendUDP_echo(packetBuffer);
          break;
        case 2:
          Serial.println("=> users online");
          logged = true;
          break;
        case 3:
          Serial.println("=> Login error");
          permanentError++;
          Serial.print("=> permanentError = ");
          Serial.println(permanentError);
          delay(1000);
          sendUDP_cmd(loginStr);
          if ( permanentError > 5 ) {
            Serial.println("permanentError > 5 => 1 minute ON HOLD");
            permanentError = 0;
            delay(60000);
          }
          break;
        default:
          // if nothing else matches, do the default
          // default is optional
          delay(10);
      }

    Serial.print("memset: ");
    Serial.println(sizeof(packetBuffer));
    memset(packetBuffer, 0, sizeof(packetBuffer));
  }
  delay(10);
}

void sendUDP_echo(char* _msg) {
  // send a reply, to the IP address and port that sent us the packet we received
  Serial.print("/send UDP echo");
  Udp.beginPacket(serverUDP, portUDP);
  Udp.write(ReplyBuffer);
  Udp.write(_msg);
  Udp.write('\r\n');
  Udp.endPacket();
  Serial.println("/sent");
  processUDP();
}

void sendUDP_cmd(char* _msg) {
  // send a reply, to the IP address and port that sent us the packet we received
  Serial.print("/send UDP cmd");
  Udp.beginPacket(serverUDP, portUDP);
  Udp.write(_msg);
  Udp.write('\r\n');
  Udp.endPacket();
  Serial.println("/sent");
  processUDP();
}

void ledBlink(int _switch) {
  if (_switch == 1) analogWrite(ledPin, 255); //PWM transforms 5V (255) to 3V (153)
  if (_switch == 0) analogWrite(ledPin, 0);
  lastLED = millis();
}

void led_reset() {
  if ( (millis() - lastLED > resetLED) && ledState != 0 ) {
    ledState = 0;
    ledState = 0;
    ledBlink(ledState);
  }
}

// heartbeat() is optional. It solves server disconnection if no data are sent for more than 10 minutes
void heartbeat() {
  if (millis() - lastHeartbeat > 30000) {
    String _str = "@RMD;00;";
    unsigned long timestamp = millis();
    int hours = timestamp - (timestamp - timestamp / 1000 / 60 / 60);
    int minutes = timestamp - (timestamp - timestamp / 1000 / 60) - hours * 60;
    int seconds = timestamp - (timestamp - timestamp / 1000) - minutes * 60 - hours * 60;
    String h = String(hours);
    String m = String(minutes);
    String s = String(seconds);
    if (hours < 10) h = '0' + h;
    if (minutes < 10) m = '0' + m;
    if (seconds < 10) s = '0' + s;

    _str = _str + h + ":" + m + ":" + s + "\r\n";
    sendUDP_echo(heartbeatBuffer);
    Serial.print(_str);
    lastHeartbeat = millis();
  }

}

