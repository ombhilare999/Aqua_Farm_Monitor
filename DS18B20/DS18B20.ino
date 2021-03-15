/////////////////////////////////////////////////////
//  Aqua Farm Monitoring System
//  Group 2:
//      - Omkar Bhilare
//      - Omkar Sargar
//      - Lukesh Ankawar
//      - Om Fuke 
////////////////////////////////////////////////////

#include <OneWire.h>
#include <DallasTemperature.h>
#include <BlynkSimpleStream.h>

#define BLYNK_PRINT Serial
#define ONE_WIRE_BUS 2          // Data wire is plugged into digital pin 2 on the Arduino

float sensorData;

char auth[] = "-----------------------";

OneWire oneWire(ONE_WIRE_BUS);          // Setup a oneWire instance to communicate with any OneWire device
DallasTemperature sensor(&oneWire);    // Pass oneWire reference to DallasTemperature library

BlynkTimer timer; // Announcing the timer


void myTimerEvent()
{
  sensorData = sensor.getTempCByIndex(0);
  Blynk.virtualWrite(V5, sensorData);
}

void setup(void)
{
  sensor.begin();  // Start up the library
  
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  
 timer.setInterval(1000L, myTimerEvent);
}

void loop(void)
{ 
  sensor.requestTemperatures(); // Send the command to get temperatures
  
  Blynk.run();
  timer.run(); // running timer every second
  
  delay(50);
}
