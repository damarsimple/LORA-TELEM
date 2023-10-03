#include <SPI.h>
#include <Wire.h>
#include <LoRa.h>
#include "boards.h"
// 0xBB or OxFF

#define MY_DEVICE_ID 0xBB
#define DESTINATION_ID 0xFF

// SWAP 

// #define MY_DEVICE_ID 0xFF
// #define DESTINATION_ID 0xBB

#define MSG_DELAY 100

String outgoing;
byte msgCount = 0;            // count of outgoing messages
byte localAddress = MY_DEVICE_ID;     // address of this device
byte destination = DESTINATION_ID;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 50;          // interval between sends

String recivcounter;
String temperature;
String humidity;

void setup()
{
  initBoard();
  // When the power is turned on, a delay is required.
  delay(1500);

  Serial.println("LoRa Receiver");

  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  if (!LoRa.begin(LoRa_frequency))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
}




void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}


void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  //--------------- Start receiving DHT22 Data --------------------

  // try to parse packet
  int pos1, pos2;

  // received a packet
  Serial.print("Received packet:  ");
  String LoRaData = LoRa.readString();
  Serial.print(LoRaData);
  // read packet
  while (LoRa.available()) {
    Serial.print((char)LoRa.read());
  }
  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());

  pos1 = LoRaData.indexOf('/');

  temperature = LoRaData.substring(0, pos1);
  humidity = LoRaData.substring(pos1 + 1, LoRaData.length());


  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(F("°C"));

  Serial.print(F("Humidity = "));
  Serial.print(humidity);
  Serial.println(F("%"));

  Serial.println();

  delay(MSG_DELAY);
}

void loop() {
  if (millis() - lastSendTime > interval) {
    //--------------- Start Sending DS18B20 Temperature Data --------------------
    float temp = 20;
    // Serial.print("DS18B20 Temperature: ");
    // Serial.print(temp);
    // Serial.println(" °C");
    // Serial.println();

    String message = String(temp);
    sendMessage(message);

    //Serial.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    interval = random(50) + 100;
  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}
