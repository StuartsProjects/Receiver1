/*
You need at least two files for a library: a header file (w/ the extension .h) and the source file (w/ extension .cpp). 
The header file has definitions for the library: basically a listing of everything that's inside; while the source 
file has the actual code. We'll call our library "Morse", so our header file will be Morse.h. Let's take a look at 
what goes in it. It might seem a bit strange at first, but it will make more sense once you see the source file that goes with it.
The core of the header file consists of a line for each function in the library, wrapped up in a class along with any variables you need:  
 */

 /*
 Morse.h - Library for flashing Morse code.
*/

#ifndef I2CDisplay_h              //this and the #endif at the bottom, prevents problems if the morse.h is included twice     
#define I2CDisplay_h

#include "Arduino.h"
#include <Wire.h>

//these are the commands that go in the first byte of a transfer on the I2C
//Some commnads are a single byte only
const byte WriteText = 0x10;
const byte CursorPosition = 0x11;
const byte ClearDisplay = 0x12;
const byte SetContrast = 0x13;
const byte SetFontSize = 0x14;
const byte InitialiseDisplay = 0x15;       //not yet implemented
const byte ClearRow = 0x16;                //not yet implemented  
byte const waitready = 1;
byte const nowaitready = 0;

class I2CDisplay : public Print
{
  public:
    I2CDisplay(int Display_Address);             //This function is public, meaning that it can be accessed by people using your library,
    void Contrast(byte lcontrast);
    void SetCurPos(byte lcol, byte lrow);
    void SetTextSize(byte lsize);
    void checkforwait();
    void setwaitReady(byte _wait);
    void waitReady();
    void clear();
    
    virtual size_t write(uint8_t _character);          //works
    //virtual size_t write(char _character);
    //virtual size_t write((const uint8_t)*s, size_t size);
    virtual size_t write(const uint8_t *s, size_t size);  //this does work
    //virtual size_t write(const char* s);    //this compiles, does not work, slow  
                                               // although write is defined twice, the ability of C to overload functions means it will call the correct
                                               // routine depending on what is passed to it  
  private:                                     //This function is private, meaning that it can only be accessed from within the class itself              
    int _Display_Address;
    byte _character_ready;
    byte _waitstatus = 1;                          //when set to 0, nowait for display to complete write, when set to 1 program waits for display to complete write, 
    int GetStatus();
};

#endif
