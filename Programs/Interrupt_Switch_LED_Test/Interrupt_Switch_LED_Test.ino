#define programname "Interrupt_Switch_LED_Test"
#define programversion "V1.0"
#define dateproduced "04/10/2017"
#define aurthorname "Stuart Robinson"

/*
********************************************************************************************************************************

  Easy Build LoRaTracker Programs for Arduino ATMEGA328

  Copyright of the author Stuart Robinson - 04/10/2017

  http://www.LoRatracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.
  
  This program uses interrupts to detect switch operation on the SWITCH1 pin. You can either define this locally or inclued the 
  appropriate board definition file, SWITCH1 and LED1 are defined for most boards.  
  
  For every SWITCH1 press the LED1 is toggled but there is no switch debounce, and you may see multiple interrupts counted. 
  
********************************************************************************************************************************
*/
#include <Arduino.h>

unsigned int SWITCH1_Interrupt_Count;

//#define SWITCH1 A1                               //local defines
//#define LED1 8                                   //local defines 

#include "Locator2_Board_Definitions.h"

#include "Program_Definitions.h"
#include "PinChangeInterrupt.h"                    //https://github.com/NicoHood/PinChangeInterrupt


void loop()
{
  
  
}


void interrupt_detected_SWITCH1()
{
 SWITCH1_Interrupt_Count++; 
 digitalWrite(LED1, !digitalRead(LED1));             //toggle the LED
 Serial.print("SWITCH1_Interrupt_Count ");                          
 Serial.println(SWITCH1_Interrupt_Count);
 delay(500);
}


void setup_interrupts()
{
  Serial.println("setup_interrupts()");
  attachPCINT(digitalPinToPCINT(SWITCH1), interrupt_detected_SWITCH1, CHANGE);
}


void led_FlashStart()
{
  byte i;
  Serial.println("led_FlashStart()");
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
  Serial.begin(38400);                       //setup Serial console ouput

  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));

  pinMode(LED1, OUTPUT);                    //for PCB LED
  led_FlashStart();

 pinMode(SWITCH1, INPUT_PULLUP);
  
  setup_interrupts();
  delay(500);
}
