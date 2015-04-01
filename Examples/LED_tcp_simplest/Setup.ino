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
