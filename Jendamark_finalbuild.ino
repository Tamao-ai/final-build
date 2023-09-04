#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10,0,0,10);
IPAddress server(127,0,0,11);

EthernetClient client;


const int DHTPIN = 2;
#define DHTTYPE DHT11


const int triggPin = 7;
const int echoPin = 8;

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;



void setup() {
  // initialize serial communication:
  Serial.begin(9600);   
  dht.begin();
  pinMode(triggPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Ethernet.begin(mac,ip);
  Serial.print("Manually assigned the following IP address to the Arduino:");
  Serial.println();
  Serial.println(Ethernet.localIP()); 
  delay(1000);
  // Check for Ethernet hardware.
  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    Serial.println("Ethernet shield was not found.");
  }
  // Check for Ethernet cable connection.
  if (Ethernet.linkStatus() == LinkOFF) 
  {
    Serial.println("Ethernet cable is not connected.");
  }
  delay(1000);

  Serial.print("Attempting connection to ");
  Serial.print(server);
  Serial.println("...");

  // Attempt to connect to the server running at IP address 192.168.1.81 and port 5000.
  if (client.connect(server,5000))
  {
    Serial.print("Connected to server running at ");
    Serial.println(client.remoteIP());
  }
  else 
  {
    Serial.println("Connection to server failed.");
  }
}

void loop() {
  
  DynamicJsonDocument doc(1024);
  

  delay(delayMS);
  sensors_event_t event;
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:

  long duration, inches, cm;

  digitalWrite(triggPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggPin, LOW);


  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(F("Distance: "));
  doc["Distance inches: "] = inches ;//+ "in, " +" "+ cm + "cm";
  Serial.print(inches);
  Serial.print("in, ");
  doc["Distance cm: "] = cm ;
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);

  // Get temperature event and print its value.
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    doc["Temperature: "] = event.temperature; //+ "°C";
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Humidity: "));
    doc["Humidity: "] = event.relative_humidity; //+ "%";
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
  Serial.println();
  delay(1000);


  serializeJson(doc, Serial);


}

long microsecondsToInches(long microseconds) {

  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {

  return microseconds / 29 / 2;
}
