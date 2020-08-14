#define programname "Memory_Test"
#define programversion "V1.0"
#define dateproduced "04/10/2017"
#define aurthorname "Stuart Robinson"

/*
********************************************************************************************************************************

  Easy Build Tracker Programs for Arduino

  Copyright of the author Stuart Robinson - 04/10/2017

  http://

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

  This program tests the non-volatile memory. At reset it first reads a float stored in the first four memory locations. It then 
  increments the value, writes it to memory and reads it back in loop. Thus if you let the test run for a while and observe the last
  printed float value, then power off or reset, on restart you can check if the values has been saved and survived power off correctly.

  After the first read the program multiplies the floats by a set value and write the new values out to memory.

  Note: If the floats calculated or retrieved are corrupted or exceed the limit then to may be print to serial terminal as;

  'nan' = not a number
  'ovf' = overflow

********************************************************************************************************************************
*/
#include <Arduino.h>


#include "I2CFRAM_MB85RC16PNF.h"                     //library file for MB85RC16PNF
//#include "EEPROM_Memory.h"                         //Library file for ATMEGA Internal EEPROM

#include "Locator2_Board_Definitions.h"          //include definitions file for board type


#include "Program_Definitions.h"
#include <Wire.h>
#include "I2C_Scanner.h"


void loop()
{
  float data;
  unsigned long cleartimemS;

  Serial.println(F("Testing Memory"));
  Serial.println();
  Print_Memory(0, 255);
  Serial.println();

  data = Memory_ReadFloat(0);
  
  Serial.print(F("Memory at Reset "));
  Serial.println(data,6);
  
  Serial.print(F("Clearing first 256 bytes of Memory "));
  cleartimemS = millis();
  Memory_Set(0, 0xff, 0);
  Serial.print(F(" - Done - "));
  cleartimemS = millis() - cleartimemS;
  Serial.print(cleartimemS);
  Serial.println(F("mS"));
  Print_Memory(0, 255);

  data = 1;

  while (1)
  {
    data = data * 1.23456;
    Serial.print(F("Writing Memory as "));
    Serial.print(data,6);
    Memory_WriteFloat(0, data);
    Serial.print(F("   Reading Memory as "));
    data = Memory_ReadFloat(0);
    Serial.println(data,6);
    delay(1000);
  }

}

void led_FlashStart()
{
  byte i;
  for (i = 0; i <= 1; i++)
  {
    digitalWrite(LED1, HIGH);
    delay(100);
    digitalWrite(LED1, LOW);
    delay(100);
  }
}





void setup()
{
  pinMode(LED1, OUTPUT); 			                           //for PCB LED
  Serial.println(F("LED Flash"));
  led_FlashStart();

  Serial.begin(38400);                                   //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println();

  setup_I2CScan();
  run_I2CScan();

  Memory_Start();

}


