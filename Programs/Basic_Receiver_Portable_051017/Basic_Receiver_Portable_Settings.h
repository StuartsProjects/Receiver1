//LoRaTracker_Settings.h
/*
******************************************************************************************************

LoRaTracker Programs for Arduino

Copyright of the author Stuart Robinson - 05/10/2017

http://www.LoRaTracker.uk
  
These programs may be used free of charge for personal, recreational and educational purposes only.

This program, or parts of it, may not be used for or in connection with any commercial purpose without
the explicit permission of the author Stuart Robinson.

The programs are supplied as is, it is up to individual to decide if the programs are suitable for the
intended purpose and free from errors.

To Do:


******************************************************************************************************
*/

//**************************************************************************************************
// 1) Hardware related definitions and options - specify board definition file type here
//**************************************************************************************************

#define Board_Definition "Locator2_Board_Definitions.h"
//#define Board_Definition "Receiver2_Board_Definitions.h"   
//#define Board_Definition "Sensor1_Board_Definitions.h"   
//#define Board_Definition "RFM98_Shield_January2016_Board_Definitions.h"
//#define Board_Definition "MicroRX_Board_Definitions.h"
//#define Board_Definition "LCD_Receiver_Board_Definitions.h"


//**************************************************************************************************
// 2) Program Options
//**************************************************************************************************

#define ConfigureDefaults               //Configure settings from default program defaults, needs to be run at least once, cleared if bind is run
//#define ClearAllMemory                //Clears from start memory to end memory, normally 1kbyte, needs to be followed by ConfigureDefaults
//#define ConfigureFromMemory           //Configure settings from values stored in memory, this needs to be the active mode for bind to work

#define CalibrateTone                     //comment in to have a calibrate tone at startup

//#define DEBUG                             //if defined debug mode used, results in more diagnostics to serial terminal    
//#define ReceiveBind                     //during flight allows for a bind to be received    



//**************************************************************************************************
// 3) Frequency settings
//**************************************************************************************************

//Tracker mode
const unsigned long TrackerMode_Frequency = 434400000;   

//Search mode
const unsigned long SearchMode_Frequency = 434300000;   

//Bind mode - Change this with great care !!!!!
const unsigned long BindMode_Frequency = 434100000;      

//this is the LoRa module frequency calibration offset in Hertz
const int CalibrationOffset = 0;                    


//**************************************************************************************************
// 4) LoRa settings
//**************************************************************************************************

#define LoRa_Device_in_MB1                  //if using a MikroBus based specify the socket for the LoRa Device 

//Tracker mode
const byte TrackerMode_Power = 10;
#define TrackerMode_Bandwidth BW62500
#define TrackerMode_SpreadingFactor SF8
#define TrackerMode_CodeRate CR45

//Search mode
const byte SearchMode_Power = 10;
#define SearchMode_Bandwidth BW62500
#define SearchMode_SpreadingFactor SF12
#define SearchMode_CodeRate CR45


//Bind mode - Change this with great care !!!!!
const byte BindMode_Power = 2;
#define BindMode_Bandwidth BW500000
#define BindMode_SpreadingFactor SF8
#define BindMode_CodeRate CR45

const byte Deviation = 0x52;    //typical deviation for tones
const byte lora_RXBUFF_Size = 128;
const byte lora_TXBUFF_Size = 1;


//#define RemoteControlNode 'G'               //normally used by tracker transmitter in promiscuous_Mode
//#define ControlledNode '1'                  //normally used by tracker transmitter in promiscuous_Mode
//#define ThisNode ControlledNode


//Protected Command Settings
const char key0 = 'L';                        //Used to restrict access to some commands, such as bind
const char key1 = 'o';
const char key2 = 'R';
const char key3 = 'a';

//#define No_DIO0_RX_Ready                    //needed for boards that cannot directly ready the DIO0 pin     

 
//**************************************************************************************************
// 5) GPS Options
//**************************************************************************************************


//#define Do_Not_Use_GPS                        //define this if you want to use receiver without a GPS attached  
#define USE_SOFTSERIAL_GPS                      //need to include this if we are using softserial for GPS 
#define GPS_in_MB2                              //if using a MikroBus board specify the socket for the GPS
//#define DebugNoGPS                            //test mode, does not use GPS used test location instead
#define Use_Test_Location                       //uses test location for local position, so you can take pictures without revealing your location

//#define GPS_Library "UBLOX_SerialGPS.h"       //define this file if a UBLOX GPS is being used
//#define GPS_Library "No_GPS.h"                //define this file if no GPS is being used
#define GPS_Library "Generic_SerialGPS.h"       //define this file if generic non-GLONASS GPS is being used

#define WhenNoGPSFix LeaveOn                    //What to do with GPS power when there is no fix at ends of wait period (LeaveOn or LeaveOff)
#define GPSPowerControl Enabled                 //Some tracker boards can remove the power form the GPS

const unsigned int GPSBaud = 9600;
const byte WaitGPSFixSeconds = 30;              //in flight mode time to wait for a new GPS fix 
const unsigned long FixisoldmS = 10000;         //if location has not updated in this number of mS, assume GPS has lost fix
const unsigned int GPSFixes = 100;              //number of GPS fixes between setting system clock from GPS   
const byte GPS_attempts = 3;                    //number of times the sending of GPS config will be attempted.
const unsigned int GPS_WaitAck_mS = 2000;       //number of mS to wait for an ACK response from GPS
const unsigned int GPSShutdownTimemS = 1900;    //Software backup mode takes around 1.9secs to power down, used in maHr calculation
const byte GPS_Reply_Size = 12;                 //size of GPS reply buffer


//Centre of Cardiff Castle keep
//const float TestLatitude = 51.48230;
//const float TestLongitude = -3.18136;
//const unsigned int TestAltitude = 48;



//Passive GPS station: Swanbridge - C1ST1667
//https://www.ordnancesurvey.co.uk/gps/legacy-control-information/C1ST1667
//N 51 23'59.105955", W 3 11',47.413031
//51.399751654166664, -3.196503619722222
const float TestLatitude = 51.399752;
const float TestLongitude = -3.196504;
const unsigned int TestAltitude = 2;


//**************************************************************************************************
// 6) Which Memory to use for storage
//**************************************************************************************************

#define Memory_Library "I2CFRAM_MB85RC16PNF.h"          //define this file if a I2C FRAM is in use
//#define Memory_Library "EEPROM_Memory.h"              //define this file if the EEPROM is being used for memory - not recommended for portable receiver  
  


//**************************************************************************************************
// 7) Display Settings 
//**************************************************************************************************

//#define Display_Library "Display_I2C_LCD.h"
//#define Use_I2C_LCD_20x4_Screens                      //use the I2C LCD Screens, 20x4 

#define Display_Library "Display_SD1306_AVR.h"
#define SD1306_SMALL_TEXT_Screens                       //use small text on display, more information   
//#define SD1306_LARGE_TEXT_Screens                     //use large text on screen, easier to read 

//#define Use_I2C_LCD                                   //use the I2C LCD Screens, 20x4  


//**************************************************************************************************
// 10) Bluetooth Options
//**************************************************************************************************

//#define Use_NMEA_Bluetooth_Uplink
//#define USE_SendOnlySoftwareSerial
#define USE_HardwareSerial                              //use hardware serial port to send Bluetooth
#define Hardware_Bluetooth_Port Serial                  //Which Hardware port to use for Bluetooth
#define BluetoothBaud 9600 

const byte Bluetooth_Buff_Size = 128;                    //size of buffer for NMEA output


//**************************************************************************************************
// 12) Miscellaneous program settings
//**************************************************************************************************

const unsigned int switch_delay = 1000;                  //delay in mS after a switch press before another can take place






