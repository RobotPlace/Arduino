void setup()
{
  Serial.begin(57600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(38400);
  delay(100);
  Serial.print("XBee ");
  xbeeSWSerialSetup();
  Serial.println();
  //Serial.println("Performed for use with: www.robotmonitor.com");
  //Serial.println("It forward data between serial and LiveData server");
  //Serial.print("source: ");
  Serial.println(sourceFile);
  //Serial.print("Name: ");
  Serial.println(robotName);
  Serial.println("DHT11");
  Serial.print("ID: ");
  Serial.println(XBeeID);
  Serial.print("ON: ");
  Serial.println(onboardSensor);
  Serial.print("PIN: ");
  Serial.println(DHTPIN);
  //Serial.println("setup START");
  // start the Ethernet connection:
  
  Serial.print("DHCP");
  if (Ethernet.begin(mac) == 0){
    //Serial.println("Fd");
    // initialize the ethernet device not using DHCP:
    Ethernet.begin(mac, robotIP);
  }
  else {
    //Serial.println(" OK");
    robotIP = Ethernet.localIP();
  }
  //Ethernet.begin(mac, robotIP);
  // print your local IP address:
  Serial.print("IP: ");
  for(int i=0; i<=3;i++){
    Serial.print(robotIP[i], DEC);
    if(i < 3) Serial.print(".");
  }
  //Serial.println();
  Serial.print("Port:");
  Serial.println(portTCP);
  dht.begin();
  //Serial.println("SD");
}

void xbeeSWSerialSetup(){
  int ATCN = 0;
  while(ATCN < 4){
    if(ATCN == 0) {
      mySerial.write("+++");
      //Serial.print(".");
      delay(2000);
      ATCN++;
    }
    else if(ATCN == 1) {
      mySerial.write("ATSH\r");
      //Serial.print(".");
      delay(100);
      ATCN++;
    }
    else if(ATCN == 2) {
      mySerial.write("ATSL\r");
      //Serial.print(".");
      delay(100);
      ATCN++;
    }
    else if(ATCN == 3) {
      mySerial.write("ATCN\r");
      //Serial.print(".");
      delay(100);
      ATCN++;
    }
    
    while(mySerial.available()){
      char c = mySerial.read();
      if(ATCN > 1 && ATCN < 4) XBeeID += c;
      //Serial.print(".");
    }
  }
  if( XBeeID.length() < 2 ) XBeeID = "123";
}
