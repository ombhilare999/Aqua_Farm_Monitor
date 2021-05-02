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
#include "DHT.h"
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 23
#define SS 18
#define RST 14
#define DIO0 2
#define BAND 866E6

String LoRaMessage = "";

#define DHTTYPE DHT11
uint8_t DHTPin = 26;                   // DHT Sensor             
DHT dht(DHTPin, DHTTYPE);             // Initialize DHT sensor.              

const int oneWireBus = 25;             // GPIO where the DS18B20 is connected to
OneWire oneWire(oneWireBus);          // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 

float temperatureC;
float Temperature,Humidity;

unsigned long interval=10000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.

//Initialize LoRa module
void startLoRA(){
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  startLoRA();
  sensors.begin();                    // Start the DS18B20 sensor

  pinMode(DHTPin, INPUT);
  dht.begin();  
}


void loop() {
  unsigned long currentMillis = millis(); // grab current time

   // check if "interval" time has passed (1000 milliseconds)
  if((unsigned long)(currentMillis - previousMillis) >= interval) {
      
    Serial.print("Sending packet: ");

    sensors.requestTemperatures(); 
    temperatureC = sensors.getTempCByIndex(0);

    Humidity = dht.readHumidity();
    Temperature = dht.readTemperature();

    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print("DHT Humidity: ");
    LoRa.print(Humidity);
    LoRa.print("::  Ambient Temperature: ");
    LoRa.print(Temperature);
    LoRa.print("::  Water Temperature: ");
    LoRa.print(temperatureC);
    LoRa.endPacket();
      
    // save the "current" time
    previousMillis = millis();
  }
}
