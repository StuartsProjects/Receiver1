/*
  You need at least two files for a library: a header file (w/ the extension .h) and the source file (w/ extension .cpp).
  The header file has definitions for the library: basically a listing of everything that's inside; while the source
  file has the actual code. We'll call our library "Morse", so our header file will be Morse.h. Let's take a look at
  what goes in it. It might seem a bit strange at first, but it will make more sense once you see the source file that goes with it.
  The core of the header file consists of a line for each function in the library, wrapped up in a class along with any variables you need:
*/

/*
  Morse.cpp - Library for flashing Morse code.
*/

#include "Arduino.h"
#include "Display_I2CBackpack.h"




/*
  Now comes the constructor. Again, this explains what should happen when someone creates an instance of your class.
  In this case, the user specifies which pin they would like to use. We configure the pin as an output save it into
  a private variable for use in the other functions:
*/


I2CDisplay::I2CDisplay(int Display_Address)      // I2CDisplay:: before the name of the function says that the function (I2CDisplay()) is part of the I2CDisplay class.
// This must match the declaration in the .h file
{
  _Display_Address = Display_Address;            // by convention add _ to make it clear which variables are private

}

/*
  These routines need to use private variables, cannot use public variables in here.
*/


//size_t I2CDisplay::write(char _character)
size_t I2CDisplay::write(uint8_t _character)
//this write routine is used if a single character is passed as in class.write('H')
{
  //Serial.print("Display Write Character");
  //Serial.write(_character);
  Wire.beginTransmission(_Display_Address);  //send backpack address
  Wire.write(WriteText);
  Wire.write(_character);
  Wire.endTransmission();
  //Serial.println();
  checkforwait();
  return 1;
}


/*size_t SSD1306Ascii::write(const char* s) {
  size_t n = strlen(s);
  for (size_t i = 0; i < n; i++) {
    write(s[i]);
  }
  return n;
}
*/
//size_t I2CDisplay::write(const char *s, size_t size)  //this does work 208ms
size_t I2CDisplay::write(const uint8_t *s, size_t size)  //this does work also 65ms
//size_t I2CDisplay::write((const uint8_t)*s, size_t size)  //
//size_t I2CDisplay::write(const char* s)             //this does not work
//this write routine is used if an array is passed as in class.print("Hello")
//note that each element of the array is sent via class.write(character)
{
  byte _character;

  Wire.beginTransmission(_Display_Address);  //send backpack address
  Wire.write(WriteText);
  
  size_t n = strlen(s);
  for (size_t i = 0; i < n; i++)
  {
    _character = s[i];
    Wire.write(_character);
    //Serial.write(_character);
  }

  Wire.endTransmission();
  Serial.println();
  checkforwait();
  return n;
}


void I2CDisplay::clear()
{
  //Serial.println("clear()");
  Wire.beginTransmission(_Display_Address);  //send backpack address
  Wire.write(ClearDisplay);
  Wire.endTransmission();
  checkforwait();
}

int I2CDisplay::GetStatus()
{
  Wire.requestFrom(_Display_Address, 1);
  _character_ready = Wire.read();
  //Serial.print(_character_ready);
  return _character_ready;
}


void I2CDisplay::waitReady()
{
  while (GetStatus());                //wait until backpack returns 0 indicating its ready for next character
  //Serial.println();
}


void I2CDisplay::setwaitReady(byte wait)
{
  _waitstatus = wait;
  //Serial.print("waitstatus ");
  //Serial.println(_waitstatus);
}

void I2CDisplay::checkforwait()
{
if (_waitstatus)
{
waitReady();
}
}


void I2CDisplay::SetTextSize(byte lsize)
{
  //Serial.print("SetTextSize() ");
  //Serial.println(lsize);
  Wire.beginTransmission(_Display_Address);  //send backpack address
  Wire.write(SetFontSize);
  Wire.write(lsize);
  Wire.endTransmission();
  checkforwait();
}


void I2CDisplay::SetCurPos(byte lcol, byte lrow)
{
  //Serial.print("SetCurPos() ");
  //Serial.print(lcol);
  //Serial.print(" ");
  //Serial.println(lrow);
  
  Wire.beginTransmission(_Display_Address);  //send backpack address
  Wire.write(CursorPosition);
  Wire.write(lcol);
  Wire.write(lrow);
  Wire.endTransmission();
  checkforwait();
}

void I2CDisplay::Contrast(byte lcontrast)
{
  //Serial.print("SetContrast() ");
  //Serial.println(lcontrast);
  Wire.beginTransmission(_Display_Address);  //send backpack address
  Wire.write(SetContrast);
  Wire.write(lcontrast);
  Wire.endTransmission();
  checkforwait();

  
}



