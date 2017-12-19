This folder contains the current version of the LoRaTracker Receiver1 programs. 

The ‘basic’ receiver programs are a cut down versions of the Receiver2 program (designed for ATMEGA1284P)
that will run on simple ATMEGA328P hardware such as the very low cost Arduino Pro Mini 3.3V.

The terminal receiver program provides the remote control interface to the HAB tracker that allows it to
be controlled and configured whilst it is in flight.

The portable receiver program displays the tracker location on a display and if a local GPS is fitted then
the receiver will show the distance and direction to the tracker. The receiver can switch between normal
tracker mode, and long distance search mode by pressing the receivers switch. The program will run on most
of the ATMEGA328P based LoRaTracker boards, including the trackers themselves, see examples in the pictures
folder. Currently supported displays are the SD1306 OLED and 20x4 LCD (3.3Version). Future display options
will be the ILI9341 TFT and Nokia 5110.

The third version of the basic receiver software is for use with boards that have an SD card connected,
the receiver then acts as a logger, recording all received packets to a file on the SD card.

The programs requre the installation of the current LoRaTracker Library files see here;

https://github.com/LoRaTracker/LoRaTracker-Library



Configuring the Receivers
-------------------------

The Receiver programs are configured using the settings file found in the programs directory. Whilst the 
programs make a number of configuration options possible, all the LoRaTracker programs tracker transmitters 
and receivers have the same basic LoRa and frequency default settings. So for a working setup all you need do
to make sure the settings file is configured as follows;

1. Select the appropriate board definition file in section '1) Hardware related definitions and options 
2. Select the appropriate GPS in section'5) GPS Options'
3. Select the appropriate display connected (in any) in section '7) Display Settings'

See 'Configuring the Receiver Programs.doc' for further details. 




