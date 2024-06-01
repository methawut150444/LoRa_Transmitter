#include <WiFi.h>
#include <LoRa.h>
#include <PCF8574.h>

PCF8574 PCF(0x20);

//define the pins used by the transceiver module
#define ss D0 //DO (esp32 XIAO)
#define rst D1 //D1 (esp32 XIAO)
#define dio0 D2 //D2 (esp32 XIAO) 

int counter = 0;
// int second = 0;
// int minute = 0;
// int hour = 0;
// int day = 0;
// String Time = "";

String module = "RED";
// String module = "BLUE";

String jsonData = "";

void setup() {
  // Serial.begin(9600);
  // Serial.println("Role: Transmitter");

  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of yout module
    // Serial.println("Starting LoRa failed!..");
    delay(100);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSpreadingFactor(10);           // ranges from 6-12,default 7 see API docs
  LoRa.setSignalBandwidth(125E3);           // for -139dB (page - 112)
  LoRa.setCodingRate4(5);                   // for -139dB (page - 112)
  LoRa.setSyncWord(0x31);
  // LoRa_setup();
  PCF8574_setup();

  //if init succeeded!
  PCF.digitalWrite(P4, LOW);
  PCF.digitalWrite(P5, LOW);
}

void loop() {

  getTime();
  jsonDataForm();

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(jsonData);
  LoRa.endPacket();

  delay(1000);
}

void LoRa_setup(){
  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of yout module
    // Serial.println("Starting LoRa failed!..");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSpreadingFactor(10);           // ranges from 6-12,default 7 see API docs
  LoRa.setSignalBandwidth(125E3);           // for -139dB (page - 112)
  LoRa.setCodingRate4(5);                   // for -139dB (page - 112)
  LoRa.setSyncWord(0x31);

  // Serial.println("LoRa init succeeded!");
}

void PCF8574_setup(){
  //Active LOW (Pull down)

  // PCF.pinMode(P0, OUTPUT);
  // PCF.pinMode(P1, OUTPUT);
  // PCF.pinMode(P2, OUTPUT);
  // PCF.pinMode(P3, OUTPUT);
  PCF.pinMode(P4, OUTPUT);
  PCF.pinMode(P5, OUTPUT);
  // PCF.pinMode(P6, OUTPUT);
  // PCF.pinMode(P7, OUTPUT);

  PCF.begin();
}

void getTime(){
  counter++;
  // second++;

  // if(second > 59){
  //   minute++;
  //   second = 0;
  // }
  // if(minute > 59){
  //   hour++;
  //   minute = 0;
  // }
  // if(hour > 23){
  //   day++;
  //   hour = 0;
  // }

  // Time = String(day) + "-" + String(hour) + "-" + String(minute) + "-" + String(second);
  // Serial.println(Time);
}

void jsonDataForm(){
  jsonData = "";

  jsonData += "{";
  jsonData += "\"board\" : \""  + module + "\", ";
  jsonData += "\"MAC Address\" : \"" + WiFi.macAddress()  + "\", ";
  jsonData += "\"time\" : \"" + String(counter)  + "\" ";
  jsonData += "}";

  // Serial.println(jsonData);
}


