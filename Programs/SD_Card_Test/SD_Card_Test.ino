//SD_Card_Test.ino

#define programname "SD_Card_Test"
#define programversion "V1.0"
#define dateproduced "041017"
#define aurthorname "Stuart Robinson"




/*
**************************************************************************************************

  Easy Build LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson - 04/10/2017

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without
  the explicit permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the
  intended purpose and free from errors.

  To use this program put a text file called 'testfile.txt' in the root directory of the SD card.
  The program will open the file and copy it to the serial console.


******************************************************************************************************
*/

#include <Arduino.h>
#include "LCD_Receiver_Board_Definitions.h"
#include "Program_Definitions.h"

#include <SPI.h>
#include <SD.h>

void loop()
{
  dump_card();
  delay(2000);
}

void dump_card()
{

  Serial.println("Opening File");

  File dataFile = SD.open("testfile.txt");        //open the test file note that only one file can be open at a time,

  if (dataFile)                                   //if the file is available, read from it
  {
    digitalWrite(LED1, HIGH);
    while (dataFile.available())
    {
      Serial.write(dataFile.read());
    }
    dataFile.close();
    Serial.println("Finished File Dump");
    digitalWrite(LED1, LOW);
  }

  else
  {
    Serial.println("Error opening testfile.txt");   //if the file isn't open print error
    error_Device();
  }
}

void  error_Device()
{
  int i;
 
   for (i = 0; i <= 9; i++)
  {
    digitalWrite(LED1, HIGH);
    delay(50);
    digitalWrite(LED1, LOW);
    delay(50);
  }
}

void led_FlashStart()
{
  byte i;
  for (i = 0; i <= 4; i++)
  {
    digitalWrite(LED1, HIGH);
    delay(150);
    digitalWrite(LED1, LOW);
    delay(150);
  }
}

void setup()
{

  pinMode(LED1, OUTPUT);                               //for PCB LED
  led_FlashStart();

  Serial.begin(38400);
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));;

  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed, or not present.");
    error_Device();
    return;                                              //loop if no card found
  }

  Serial.println("Card initialized.");

}


