// String datas = linkStart  linkSeparator  linkEnd

void dataProcess()
{
  if(millis()-lastdataRead > dataReadPeriod){
    if(pr) {
      unsigned long s = millis()/1000;
      if(s/60/60 < 10) Serial.print("0");
      Serial.print(s/60/60); //hours
      Serial.print(":");
      if(s/60 < 10) Serial.print("0");
      Serial.print(s/60); //minutes
      Serial.print(":");
      if(s < 10) Serial.print("0");
      Serial.print(s); //seconds
      Serial.print("   ");
    }
    readData();
    //send data only if changed and every 60 seconds
    if( (h != hLast ||  t != tLast) && millis()-lastdataSend > dataReadPeriod*30){
      hLast = h;
      tLast = t;
      String datas = "{%22id%22:%22" + XBeeID + "%22,%22" + dataTableName + "%22:[" + String(t) + "," + String(h) + "]}";
      if(pr) Serial.print("  change  ");
      Serial.println(datas); //XBEE * SoftwareSerial *
      lastdataSend = millis();
    }
    lastdataRead = millis();
  }
}

void readData()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int hTemp = h;
  int tTemp = t;
  h = dht.readHumidity();
  t = dht.readTemperature();
  //make a mean from data
  h = (h+hTemp)/2.;
  t = (t+tTemp)/2.;
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    if(pr) Serial.println("Failed DHT");
  }
  else {
    // Serial.print("Temperature: ");
    if(pr) Serial.print(t);
    if(pr) Serial.print(" *C, ");
    //Serial.print("Humidity: ");
    if(pr) Serial.print(h);
    if(pr) Serial.println(" %\t ");
  }
}




