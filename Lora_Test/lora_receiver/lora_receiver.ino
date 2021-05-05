/////////////////////////////////////////////////////
//  Aqua Farm Monitoring System
//  Group 2:
//      - Omkar Bhilare
//      - Omkar Sargar
//      - Lukesh Ankawar
//      - Om Fuke 
////////////////////////////////////////////////////

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


char auth[] = "T_NiFIv-ulqUcA4vrA54FE2BcOni1Isa";
char ssid[] = "iBall-Baton";
char pass[] = "1234567847";

// Attach virtual serial terminal to Virtual Pin V9
WidgetTerminal terminal(V9);

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
int counter = 0;
String LoRaMessage = "";

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

BlynkTimer timer;

void myTimerEvent()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    if (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData); 
      terminal.println(LoRaData);      
      // Ensure everything is sent
      terminal.flush();
    }
  }
}  
void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  startLoRA();

  Blynk.begin(auth, ssid, pass);

  // Clear the terminal content
  terminal.clear();

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.flush();
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  // try to parse packet
  Blynk.run();
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    if (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData); 
      terminal.println(LoRaData);      
      // Ensure everything is sent
      terminal.flush();
    }
  }
}
