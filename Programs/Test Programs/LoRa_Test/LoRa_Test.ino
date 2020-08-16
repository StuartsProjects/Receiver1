//LoRa_Test.ino
/*
**************************************************************************************************

  Tracker Programs for Arduino

  Copyright of the author Stuart Robinson

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without
  the explicit permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

  Test program for LoRa device, prints registers and transmits an FM tone on selected frequency.
  
**************************************************************************************************
*/

#define programname "LoRa_Test"
#define programversion "V1.0"
#define dateproduced "031017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>
#include "Program_Definitions.h"

byte keypress;                               //used in LoRa routines, so needs to be defined
const byte lora_RXBUFF_Size = 1;
const byte lora_TXBUFF_Size = 1;
const byte Deviation = 0x52;                 //typical deviation for tones


#include "Locator2_Board_Definitions.h"

#include <SPI.h>
#include "LoRa3.h"


#define TrackerMode_Frequency 434400000      //transmit frequency in hertz for tones
#define TrackerMode_Power 10
#define CalibrationOffset 0                  //this is the LoRa module frequency calibration offset in Hertz

unsigned int seq = 1;


void loop()
{

  Serial.print("Start Test Loop ");
  Serial.println(seq++);
  Serial.println();

  Setup_TrackerMode();

  lora_Print();
  Serial.println();
  Serial.println(F("LoRa Tone"));
  lora_Tone(1000, 3000, 10);                  //Transmit an FM tone, 1000hz, 3000ms
  delay(2000);
}


void Setup_TrackerMode()
{
  //sets LoRa modem to Tracker mode
  lora_SetFreq(TrackerMode_Frequency, CalibrationOffset);
}



void led_FlashStart()
{
  byte i;
  for (i = 0; i <= 4; i++)
  {
    digitalWrite(LED1, HIGH);
    delay(250);
    digitalWrite(LED1, LOW);
    delay(250);
  }
}


void systemerror()
{
  while (1)
  {
    digitalWrite(LED1, HIGH);
    delay(100);
    digitalWrite(LED1, LOW);
    delay(100);
  }
}



void setup()
{
  Serial.begin(38400);                        //setup Serial console output
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();

  pinMode(LED1, OUTPUT);                     //for PCB LED
  led_FlashStart();

  pinMode(lora_NReset, OUTPUT);			        //LoRa Device reset pin
  pinMode (lora_NSS, OUTPUT);			          //LoRa Device select pin
  digitalWrite(lora_NSS, HIGH);
  digitalWrite(lora_NReset, HIGH);

  SPI.begin();				                      //initialize SPI

  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  if (lora_CheckDevice() == true)
  {
    lora_Setup();			                      //Do the initial LoRa Setup
    Setup_TrackerMode();

    Serial.println();
  }
  else
  {
    Serial.println(F("LoRa Device Error"));
    systemerror();
  }


}





