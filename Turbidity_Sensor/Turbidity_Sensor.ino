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


char auth[] = "----------------------------------";
char ssid[] = "---------------";
char pass[] = "------";

// Attach virtual serial terminal to Virtual Pin V9
WidgetTerminal terminal(V9);

const int oneWireBus = 25;             // GPIO where the DS18B20 is connected to
float temperatureC;
OneWire oneWire(oneWireBus);          // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 

#define DHTTYPE DHT11


uint8_t DHTPin = 26;                   // DHT Sensor             
DHT dht(DHTPin, DHTTYPE);             // Initialize DHT sensor.              
float Temperature,Humidity;

double turbidity;                     //Sensor data
BlynkTimer timer;

void myTimerEvent()
{
  sensors.requestTemperatures(); 
  temperatureC = sensors.getTempCByIndex(0);

  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature();

  int val = analogRead(34);
  turbidity = map(val, 0, 2200, 100, 1);

  if (turbidity < 5) {
    terminal.println("Clear Water");
  }
  else if((turbidity > 7) && (turbidity < 20)) {
    terminal.println("Cloudy Water");
  }
  else if (turbidity > 20) {
    terminal.println("Dirty Water");
  }

  // Ensure everything is sent
  terminal.flush();
  
  Blynk.virtualWrite(V5, temperatureC);
  Blynk.virtualWrite(V6, Temperature);
  Blynk.virtualWrite(V7, Humidity);
  Blynk.virtualWrite(V8, abs(turbidity));
}


void setup() { 
  Serial.begin(115200);               // Start the Serial Monitor
  sensors.begin();                    // Start the DS18B20 sensor

  pinMode(DHTPin, INPUT);
  dht.begin();  
  
  Blynk.begin(auth, ssid, pass);

  // Clear the terminal content
  terminal.clear();

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
