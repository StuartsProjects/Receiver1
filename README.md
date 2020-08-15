This repository contains the current version of the Tracker Receiver1 programs. 

See the Programs folder for the details of the programs and how to configure them. 

The ‘basic’ receiver programs are a cut down versions of the Receiver2 program (designed for ATMEGA1284P)
that will run on simple ATMEGA328P hardware such as the very low cost Arduino Pro Mini 3.3V.

The terminal receiver program provides the remote control interface to the HAB tracker that allows it to
be controlled and configured whilst it is in flight.

The portable receiver program displays the tracker location on a display and if a local GPS is fitted then
the receiver will show the distance and direction to the tracker. The receiver can switch between normal
tracker mode, and long distance search mode by pressing the receivers switch. The program will run on most
of the ATMEGA328P based Tracker boards, including the trackers themselves, see examples in the pictures
folder. Currently supported displays are the SD1306 OLED and 20x4 LCD (3.3Version). Future display options
will be the ILI9341 TFT and Nokia 5110.

The third version of the basic receiver software is for use with boards that have an SD card connected,
the receiver then acts as a logger, recording all received packets to a file on the SD card.

Most of the Tracker boards can be used as a receiver, see the document;

'Tracker Receivers.doc' 

There are build instruction here for the LCD and Mikrobus based receiver. 

The receiver programs need the Tracker Library installed see here;

https://github.com/StuartsProjects/Tracker-Library

Address of this repository https://github.com/StuartsProjects/Receiver1

Stuart Robinson
December 2017
