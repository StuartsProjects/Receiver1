This folder contains the current version of the LoRaTracker Receiver1 programs. 

The ‘basic’ receiver programs are a cut down versions of the Receiver2 program (designed for ATMEGA1284P)
that will run on simple ATMEGA328P hardware such as the very low cost Arduino Pro Mini 3.3V.

The terminal receiver program provides the remote control interface to the HAB tracker that allows it to
be controlled and configured whilst it is in flight.

The portable receiver program displays the tracker location on a display and if a local GPS is fitted then
the receiver will show the distance and direction to the tracker. The receiver can switch between normal
tracker mode, and long distance search mode by pressing the switch. The program will run on most of the 
ATMEGA328P based LoRaTracker boards, including the trackers themselves, see the pictures folder. Currently
supported displays are the SD1306 OLED and 20x4 LCD (3.3V!) when driven by an avaialble PCF8574 I2C IO expander.
Future display options will be the Nokia 5110 and ILI9341 via a serial backpack and the Digole displays. 


The third version of the basic receiver software is for use with boards that have an SD card connected,
the receiver then acts as a logger, recording all received packets to a file on the SD card.  

The programs requre the installation of the current LoRaTracker Library files see here;

https://github.com/LoRaTracker/LoRaTracker-Library

