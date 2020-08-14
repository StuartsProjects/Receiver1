//SD1306_Display_Test.ino
/*
**************************************************************************************************

  Easy Build Tracker Programs for Arduino

  Copyright of the author Stuart Robinson - 05/10/2017

  http://

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

  This test program uses the ASCII only SD1306 driver library; //https://github.com/greiman/SSD1306Ascii
 **************************************************************************************************

*/


#include "Display_SD1306_AVR.h"            //Tracker Library file that uses text only library for low memory footprint
unsigned int count = 0;


void loop(void) 
{
  Serial.println(F("Running Display Test"));
  Display_SetCurPos(0, 0);
  Display_SetTextSize(1);
  disp.print("");
  Display_SetCurPos(0, 1);
  disp.print("Hello World!");
  Display_SetCurPos(0, 2);
  disp.print("Small Text");
  Display_SetTextSize(2);
  Display_SetCurPos(0, 5);
  disp.print("Large Text");
  Display_Update();
  delay(2000);
  Display_Clear();
  Display_Update();
  delay(1000);
}


void setup(void) 
{
  Serial.begin(38400);
  Serial.print("SD1306 Display Test");

  Display_Setup();
}





