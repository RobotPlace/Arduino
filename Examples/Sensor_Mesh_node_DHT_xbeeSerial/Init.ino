#include <SoftwareSerial.h>
//DHT-sensor-library
#include <DHT.h> // download at https://github.com/adafruit/DHT-sensor-library

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

#include <SPI.h>

// device
String robotName = "#59 My Sensor mesh";
String sourceFile = "i141104_Mesh_node_DataTable_DHT_SerialXBee";
String dataTableName = "Temp_RH";
String dataProcessInfo = "dataProcess()";
unsigned long dataReadPeriod = 2000;
unsigned long lastdataRead = 0;
unsigned long lastdataSend = 0;
String bufferData = "";
int hLast = 0;
int tLast = 0;
int h = 0;
int t = 0;

String XBeeID = "";
SoftwareSerial mySerial(10, 11); // RX, TX

boolean pr = false; //print Serial ?
