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
  
  This program uses interrupts to detect switch presse and the Mikrobus socket interrupt pins 
  
  For every SWITCH1 press the LED1 is toggled but there is no switch debounce. 
  
********************************************************************************************************************************
*/
#include <Arduino.h>

//#include "LCD_Receiver_Board_Definitions.h"
#include "Locator2_Board_Definitions.h"
#include "Program_Definitions.h"


void loop()
{

 if (!digitalRead(A0))
 {
 digitalWrite(LED1, HIGH); 
 }
 else
 {
 digitalWrite(LED1, LOW);
 } 


if (!digitalRead(A1))
 {
 digitalWrite(LED1, HIGH); 
 }
 else
 {
 digitalWrite(LED1, LOW);
 } 


if (!digitalRead(13))
 {
 digitalWrite(LED1, HIGH); 
 }
 else
 {
 digitalWrite(LED1, LOW);
 } 

  
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

  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
}
