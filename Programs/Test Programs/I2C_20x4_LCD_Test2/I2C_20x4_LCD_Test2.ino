//I2C_20x4_LCD_Test.ino
/*
**************************************************************************************************

  LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without
  the explicit permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

  Test program for 20x4 LCD displa, driven via the PCF8563 I2C I\O expander 

  
**************************************************************************************************
*/
#include <Wire.h>                                               //Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>                                  //https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/ (version 1.3.4)
#include "I2C_Scanner.h"

LiquidCrystal_I2C disp(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //Set the LCD I2C address and pins used

void loop()
{
disp.setCursor(0,0);                 //Start at character 4 on line 0
disp.print("www.LoRaTracker.uk");
delay(1000);
disp.setCursor(0,1);
disp.print("I2C LCD Backpack");      //Print text on 2nd Line
delay(1000);
disp.setCursor(0,2);
disp.print("0123456789ABCDEFGHIJ");  //Print 20 characters on 3rd line
delay(1000);
disp.setCursor(0,3);
disp.print("Last Line of Text");
delay(1000);
disp.clear();
disp.setCursor(0,0);                //Start at character 4 on line 0
disp.print("LAT 51.48230   48M");
disp.setCursor(0,1);
disp.print("LON  -3.18136");        
delay(1000);
disp.setCursor(0,2);
disp.print("9.2k  6D");             
delay(1000);
disp.setCursor(0,3);
disp.print("SNR 8dB     1    SEG");
delay(5000);
disp.clear();
}
  

void setup()
{
  Serial.begin(38400);  
  setup_I2CScan();
  run_I2CScan();
  disp.begin(20,4);                    //initialize the lcd for 20 chars 4 lines, turn on backlight
  disp.backlight();                    //backlight on  
}




