/*
   This is a very simpel program that will read characters from the GPS and put them on the serial monitor
   If your ever having problems with a GPS (or just think you are) use this program first.

   If you get no data disaplayed on the serial monitor, the most likley cause is that you dont have the RX
   pin connected properly.

   If the data displayed on the serial terminal appears to be random text its very likley you have the GPS
   serial baud rate set incorrectly.
*/

#define GPSBAUD 9600                  //this is the serial baud rate that will be used for the GPS
#define MONITORBAUD 115200            //this is the serial baud rate that will be used for the serial monitor 

#include "Locator2_Board_Definitions.h"

#include <NeoSWSerial.h>              //https://github.com/SlashDevin/NeoSWSerial  
NeoSWSerial GPS(GPSRX, GPSTX);        //this library is more relaible at GPS init than software serial


void loop()
{
  while (GPS.available())
  {
    Serial.write(GPS.read());
  }
}


void setup()
{
  pinMode(GPSPOWER, OUTPUT);             //setup pin for GPS power control, if fitted
  digitalWrite(GPSPOWER, LOW);
  
  GPS.begin(GPSBAUD);
  Serial.begin(MONITORBAUD);
  Serial.println("Simple GPS Echo Starting");
 
}
