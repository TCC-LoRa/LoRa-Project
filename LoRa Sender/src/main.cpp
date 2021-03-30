#include "heltec.h"
#include "images.h"
#include <iostream>
#include <vector>
#include <string>

#define BAND 433E6 //you can set band here directly,e.g. 868E6,915E6

//Declara a Array de Strings para manipular os bytes (tamanho do pacote de teste)
char bytes_array[50];
String bytes_string;
unsigned int id_byte = 1;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0, 5, logo_width, logo_height, logo_bits);
  Heltec.display->display();
}

String convertToString(char *bytes_array, int size)
{
  String string = "";
  for (int i = 0; i < size; i++)
  {
    string = string + bytes_array[i];
  }
  return string;
}

void setup()
{
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_16);

  logo();
  delay(1500);
  Heltec.display->clear();

  Heltec.display->drawStringMaxWidth(0, 0, 128, "LoRa Iniciado com sucesso!");
  Heltec.display->display();
  LoRa.setSyncWord(0xF3);

  //Popula o Array de bytes com os parâmetros do teste desejado
  for (int i = 0; i < 50; i++)
  {
    bytes_array[i] = 'a';
  }

  //Converte a array para String para poder enviar
  bytes_string = convertToString(bytes_array, 50);

  delay(1000);
}

void loop()
{
  if (id_byte <= 5)
  {
    Heltec.display->clear();

    Heltec.display->drawStringMaxWidth(0, 0, 128, "Enviando pacote: ");
    Heltec.display->drawStringMaxWidth(64, 20, 128, String(id_byte));
    Heltec.display->display();

    /*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
    LoRa.beginPacket();
    LoRa.setTxPower(20, RF_PACONFIG_PASELECT_PABOOST); //20dB output must via PABOOST
    LoRa.print(String(id_byte) + ";" + bytes_string);
    LoRa.endPacket();

    id_byte++;
    digitalWrite(LED, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000);             // wait for a second
    digitalWrite(LED, LOW);  // turn the LED off by making the voltage LOW
    delay(5000);             // wait for a second
  }
  else
  {
    //Envia sinal de que terminou os testes
    LoRa.beginPacket();
    LoRa.setTxPower(20, RF_PACONFIG_PASELECT_PABOOST);
    LoRa.print("Fim");
    LoRa.endPacket();

    Heltec.display->clear();
    Heltec.display->drawStringMaxWidth(0, 0, 128, "Finalizou os testes!");
    Heltec.display->display();
  }
}
