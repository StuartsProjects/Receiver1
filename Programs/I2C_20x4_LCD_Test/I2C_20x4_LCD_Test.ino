#define programname "I2C_20x4_LCD_Test"
#define programversion "V1.1"
#define dateproduced "29/11/2017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>

/*
*****************************************************************************************************************************
Tracker Test Programs

Copyright of the author Stuart Robinson - 29/11/2017

These programs may be used free of charge for personal, recreational and educational purposes only.

This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
of the author Stuart Robinson.

The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
free from errors.
*****************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Program operation

This is a test program for a 20x4 LCD display with HD44870 controller driven via the PCF8563 I2C I\O expander. Back packs that fit 
on the LCD display are readily available at low cost. 

First the program scans the I2C bus for devices, it should report a device found at 0x3F, this is the PCF8563.

Then you should see this as the first scree on the LCD, assuming you have adjusted the contrast correctly. 

Hello World
I2C LCD Backpack
0123456789ABCDEFGHIJ
Last Line of Text

********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
Connections

The program uses the standard I2C connections on Arduino.
********************************************************************************************************************************
*/



#include <Wire.h>                                                //Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>                                   //www.4tronix.co.uk/arduino/sketches/LiquidCrystal_V1.2.1.zip
#include "I2C_Scanner.h"
#define PCF8574Address 0x3F                                      //address of PCF8574 can change, I have noted addresses of 0x27 and 0x3F

LiquidCrystal_I2C disp(PCF8574Address, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //Set the LCD I2C address and pins used

#define Serial_Monitor_Baud 38400                                //this is baud rate used for the Arduino IDE Serial Monitor

void loop()
{
screen1();
delay(2000);
disp.clear();
screen2();
delay(5000);
disp.clear();
screen3();
delay(5000);
disp.clear();
}
  

void screen1()
{
disp.setCursor(0,0);                //Start at character 4 on line 0
disp.print("Hello World");
delay(500);
disp.setCursor(0,1);
disp.print("I2C LCD Backpack");     //Print text on 2nd Line
delay(500);
disp.setCursor(0,2);
disp.print("0123456789ABCDEFGHIJ"); //Print 20 characters on 3rd line
delay(500);
disp.setCursor(0,3);
disp.print("Last Line of Text");
}


void screen2()
{
disp.setCursor(0,0);                
disp.print("LAT 51.48230  48M");
disp.setCursor(0,1);
disp.print("LON -3.18136");         
disp.setCursor(0,2);
disp.print("9.2K 6D");             
disp.setCursor(0,3);
disp.print("SNR 8dB     1    SEG");
}


void screen3()
{
/*
copy of LoRa messages received from NORSAT2 in orbit during The Things Network conferance, Amsterdam, 2018.
Transmited at 162Mhz but received on a 434Mhz module !!!
SNR,-18dB  RSSI,-107dB let's build this thing together
SNR,-19dB  RSSI,-109dB TTNSAT calling here
SNR,-18dB  RSSI,-109dB TTNSAT calling here
SNR,-17dB  RSSI,-108dB let's bu
SNR,-18dB  RSSI,-108dB hello from space, TTN! * 
*/

disp.setCursor(0,0);                
disp.print("hello from space,TTN");
disp.setCursor(0,1);
disp.print("TTNSAT calling here");         
disp.setCursor(0,2);
disp.print("let's build this thi");             
disp.setCursor(0,3);
disp.print("SNR -18dB");
}



void setup()
{
  Serial.begin(Serial_Monitor_Baud);  

  setup_I2CScan();
  run_I2CScan();

  disp.begin(20,4);                    //initialize the lcd for 20 chars 4 lines, turn on backlight
  disp.backlight();                    //backlight on  
}
