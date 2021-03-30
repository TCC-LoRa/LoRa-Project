#include "heltec.h"
#include "images.h"
#include "string.h"

#define BAND 433E6 //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packetsReceived;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0, 5, logo_width, logo_height, logo_bits);
  Heltec.display->display();
}

void LoRaData(String packet)
{
  Heltec.display->clear();

  if (packet == "Fim")
  {
    Heltec.display->drawStringMaxWidth(0, 0, 128, "Pacotes recebidos: " + packetsReceived);
    Heltec.display->display();
  }
  else
  {
    Heltec.display->drawStringMaxWidth(0, 0, 128, rssi);
    Heltec.display->drawStringMaxWidth(0, 16, 128, "Recebi " + packSize + " bytes");
    Heltec.display->drawStringMaxWidth(0, 32, 128, "Pacote: " + packet);
    Heltec.display->display();
  }
}

void parsePacket(int packetSize)
{
  String packet = "";
  String parsed;
  packSize = String(packetSize, DEC);
  for (int i = 0; i < packetSize; i++)
  {
    packet += (char)LoRa.read();
  }

  for (int i = 0; i < packet.length(); i++)
  {
    parsed = packet[i];

    if (packet[i] != ';')
    {
      break;
    }
  }

  rssi = "RSSI: " + String(LoRa.packetRssi(), DEC);
  if (packet == "Fim")
  {
    LoRaData(packet);
  }
  else
  {
    packetsReceived += parsed + ' ';
    LoRaData(parsed);
  }
}

void setup()
{
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_16);
  logo();
  delay(1500);
  Heltec.display->clear();

  Heltec.display->drawStringMaxWidth(0, 0, 128, "Esperando por um pacote...");
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  LoRa.setSyncWord(0xF3);
  LoRa.receive();
}

void loop()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    parsePacket(packetSize);
  }
  delay(10);
}