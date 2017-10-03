This folder contains the current version of the LoRaTracker Receiver1 programs. 

The ‘basic’ receiver programs are a cut down versions of the Receiver2 program (designed for ATMEGA1284P)
that will run on simple ATMEGA328P hardware such as the very low cost  Arduino Pro Mini 3.3V.

The terminal receiver program provides the remote control interface to the HAB tracker that allows it to
be controlled and configured whilst it is in flight.

There are two other versions of the basic receiver software, to be published shortly, one designed for
portable operation that will use a locally attached GPS and when it receives packets from the remote HAB
will on an attached display show the location and direction to the tracker.  

The third version of the basic receiver software is for use with boards that have an SD card connected,
the receiver then acts as a logger, recording all received packets to a file on the SD card.  

The programs requre the installation of the current LoRaTracker Library files see here;

https://github.com/LoRaTracker/LoRaTracker-Library

The programs in here are;

Basic_Receiver_Terminal_xxxxxx - A HAB tracker receiver that uses a serial terminal for display and remote control of the HAB tracker

I2C_Scanner - a basic I2C scanner, reports any I2C devices found
