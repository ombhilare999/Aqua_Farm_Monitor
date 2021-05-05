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

//packet counter
int readingID = 0;
String LoRaMessage = "";
String water = "";

#define DHTTYPE DHT11
uint8_t DHTPin = 26;                   // DHT Sensor             
DHT dht(DHTPin, DHTTYPE);             // Initialize DHT sensor.              

const int oneWireBus = 25;             // GPIO where the DS18B20 is connected to
OneWire oneWire(oneWireBus);          // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature sensor 

float temperatureS;
float Temperature,Humidity;

double turbidity;                     //Sensor data

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

void getReadings(){
  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature();
  sensors.requestTemperatures(); 
  temperatureS = sensors.getTempCByIndex(0);
  
  int val = analogRead(34);
  turbidity = map(val, 0, 2200, 100, 1);

  if (turbidity < 5) {
    Serial.print("Clear Water  ");
    water = "Clear";
  }
  else if((turbidity > 7) && (turbidity < 30)) {
    Serial.print("Cloudy  Water  ");
    water = "Cloudy";
  }
  else if (turbidity > 30) {
    Serial.print("Dirty Water  ");
    water = "Dirty";
  }
  
  Serial.print("Temperature :  ");
  Serial.print(Temperature);
  Serial.print("  ::  Humidity :  ");
  Serial.print(Humidity);
  Serial.print("  ::  Submereged Temperature :  ");
  Serial.println(temperatureS);
}

void sendReadings() {
  LoRaMessage = "T " + String(Temperature) + "  H " + String(Humidity) + "  ST " + String(temperatureS) + "  W  "+ water;
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
  
  Serial.print("Sending packet: ");
  Serial.println(readingID);
  readingID++;
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
  getReadings();
  sendReadings();
  delay(1000);
}
