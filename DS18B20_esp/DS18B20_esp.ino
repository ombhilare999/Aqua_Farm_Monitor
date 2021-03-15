/////////////////////////////////////////////////////
//  Aqua Farm Monitoring System
//  Group 2:
//      - Omkar Bhilare
//      - Omkar Sargar
//      - Lukesh Ankawar
//      - Om Fuke 
////////////////////////////////////////////////////

#define BLYNK_PRINT Serial
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


char auth[] = "-----------------------";
char ssid[] = "----------";
char pass[] = "----------";

const int oneWireBus = 25;             // GPIO where the DS18B20 is connected to
float temperatureC;
OneWire oneWire(oneWireBus);          // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 

#define DHTTYPE DHT11


uint8_t DHTPin = 26;                   // DHT Sensor             
DHT dht(DHTPin, DHTTYPE);             // Initialize DHT sensor.              
float Temperature,Humidity;

BlynkTimer timer;

void myTimerEvent()
{
  sensors.requestTemperatures(); 
  temperatureC = sensors.getTempCByIndex(0);

  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature();
  
  Blynk.virtualWrite(V5, temperatureC);
  Blynk.virtualWrite(V6, Temperature);
  Blynk.virtualWrite(V7, Humidity);
}


void setup() { 
  Serial.begin(115200);               // Start the Serial Monitor
  sensors.begin();                    // Start the DS18B20 sensor

  pinMode(DHTPin, INPUT);
  dht.begin();  
  
  Blynk.begin(auth, ssid, pass);
  
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
