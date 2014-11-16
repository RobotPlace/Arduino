void setup()
{
  Serial.begin(38400);//Serial 57600, XBEE * SoftwareSerial 38400*
  mySerial.begin(38400);
  delay(1000);
  Serial.print("Setup XBee ");
  //xbeeSetupSWSerial();
  xbeeSetupSerial();
  Serial.println();
  Serial.print("XBeeID: ");
  Serial.println(XBeeID);
  //Serial.println("DEVICE INFO");
  //Serial.println("Performed for use with: www.robotmonitor.com");
  //Serial.println("It forward data between serial and LiveData server");
  Serial.println(sourceFile);
  Serial.print("robotName: ");
  Serial.println(robotName);
  Serial.println("onboardSensor: DHT11");
  Serial.print("onboardSensor ID: ");
  Serial.println(XBeeID);
  Serial.print("onboardSensor PIN: ");
  Serial.println(DHTPIN);
  dht.begin();
  //initial reading
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.println("Setup END");
}

void xbeeSetupSWSerial(){
  delay(1000);
  int ATCN = 0;
  while(ATCN < 4){
    if(ATCN == 0) {
      mySerial.write("+++");
      Serial.print(".");
      delay(2000);
      ATCN++;
    }
    else if(ATCN == 1) {
      mySerial.write("ATSH\r");
      Serial.print(".");
      delay(500);
      ATCN++;
    }
    else if(ATCN == 2) {
      mySerial.write("ATSL\r");
      Serial.print(".");
      delay(500);
      ATCN++;
    }
    else if(ATCN == 3) {
      mySerial.write("ATCN\r");
      Serial.print(".");
      delay(500);
      ATCN++;
    }
    
    while(mySerial.available()){
      char c = mySerial.read();
      if(ATCN > 1 && ATCN < 4) XBeeID += c;
      Serial.print(".");
    }
  }
  if( XBeeID.length() < 2 ) XBeeID = "123456789";
}

void xbeeSetupSerial(){
  delay(2000);
  String XBeeID2 = "";
  int ATCN = 0;
  int i = 0;
  while(ATCN < 4){
    if(ATCN == 0) {
      Serial.write("+++");
      delay(4000);
      ATCN++;
    }
    else if(ATCN == 1) {
      Serial.write("ATSH\r");
      i = 0;
      delay(500);
      ATCN++;
    }
    else if(ATCN == 2) {
      Serial.write("ATSL\r");
      i = 0;
      delay(500);
      ATCN++;
    }
    else if(ATCN == 3) {
      Serial.write("ATCN\r");
      delay(500);
      ATCN++;
    }
    
    while(Serial.available()){
      char c = Serial.read();
      if(ATCN == 2 && i <= 5) {XBeeID += c; i++;}
      else if(ATCN == 3 && i <= 7) {XBeeID += c; i++;}
      if(ATCN > 1 && ATCN < 4) XBeeID2 += c;
    }
  }
  Serial.println();
  if( XBeeID.length() < 2 ) XBeeID = "123456789";
}


