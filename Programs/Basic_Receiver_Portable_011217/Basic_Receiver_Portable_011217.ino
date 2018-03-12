//**************************************************************************************************
// Note:
//
// Make changes to this Program file at your peril
//
// Configuration changes should be made in the LoRaTracker_Settings file not here !
//
//**************************************************************************************************

#define programname "Basic_Receiver_Portable_011217"
#define programversion "V2.1"
#define aurthorname "Stuart Robinson - www.LoRaTracker.uk"

#include <Arduino.h>
#include <avr/pgmspace.h>

#include "Program_Definitions.h"
#include "Basic_Receiver_Portable_Settings.h"

/*
**************************************************************************************************

  LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson - 01/12/2017

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without
  the explicit permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the
  intended purpose and free from errors.

  This is a basic portable receiver that will run on an ATMEGA328P, it picks up the packets form the 
  tracker and displays them. There is no two way comminications functionality, apart from the bind function. 
  The program is a trimmed version of the Receiver2 program.

  To make the program compatible with LoRaTracker boards that use A1 a switch input the switch is polled, 
  avoiding an interrupt conflict between NeoSWserial and pin change interrupt. 



  To do:
  Check there is Display_Setup() in all libraries
  Check need for print_CurrentLoRaSettings()
  Why record_LocalData(); save_LocalData(); distanceto(); directionto(); called when def for no GPS

  ******************************************************************************************************
*/


//Program Variables
int ramc_CalibrationOffset;                        //adjustment for frequency in Hertz

unsigned long ramc_TrackerMode_Frequency;          //Initial default for FlightFrequency
byte ramc_TrackerMode_Bandwidth;                   //used to keep Tracker mode settings
byte ramc_TrackerMode_SpreadingFactor;
byte ramc_TrackerMode_CodeRate;
byte ramc_TrackerMode_Power;

unsigned long ramc_SearchMode_Frequency;           //Initial default for FlightFrequency
byte ramc_SearchMode_Bandwidth;                    //used to keep Searc mode settings
byte ramc_SearchMode_SpreadingFactor;
byte ramc_SearchMode_CodeRate;
byte ramc_SearchMode_Power;

float TRLat;                                       //tracker transmitter co-ordinates
float TRLon;
unsigned int TRAlt;
byte TRStatus;                                     //Status byte from tracker binary payload
unsigned int TRdirection;                          //Tracker direction in degrees
float TRdistance;
boolean Remote_GPS_Fix = false;                    //set if we have received a location from the remote tracker
boolean GLONASS_Active;
boolean GPS_Config_Error;

float LocalLat;                                    //local GPS co-ordinates
float LocalLon;
unsigned int LocalAlt;
boolean Local_GPS_Fix = false;                     //set if there is a Local GPS fix at last check

byte Switchpress = 0;                              //set when a switch is pressed, 0 if not otherwise 1-5
byte Function_Number = 1;                          //identifiews the current function

char keypress;                                     //records the keypress from serial terminal
byte modenumber;                                   //records which set of LoRa settings in use, 1 = tracker, 2 = search, 3 = command, 4=bind

unsigned long TrackerMode_Packets;                 //running count of trackermode packets received
unsigned int SearchMode_Packets;                   //running count of searchmode packets received
unsigned int SupplyVolts;


//File Includes

#include Board_Definition                         //include previously defined board definition file
#include Display_Library                          //include previously defined Display Library

#include Memory_Library                           //include previously defined Memory Library

#include <TinyGPS++.h>                            //http://arduiniana.org/libraries/tinygpsplus/
TinyGPSPlus gps;                                  //Create the TinyGPS++ object
TinyGPSCustom GNGGAFIXQ(gps, "GNGGA", 5);         //custom sentences used to detect possible switch to GLONASS mode


#ifdef USE_SOFTSERIAL_GPS
#include <NeoSWSerial.h>                          //https://github.com/SlashDevin/NeoSWSerial  
NeoSWSerial GPSserial(GPSRX, GPSTX);              //this library is more relaible at GPS init than software serial
#endif

#ifndef Do_Not_Use_GPS
#include GPS_Library                              //include previously defined GPS Library
#endif

#include <SPI.h>
#include <Wire.h>
#include "LoRa3.h"
#include Display_Screens                           //include previously defined screens 

#ifdef USE_SendOnlySoftwareSerial
#include <SendOnlySoftwareSerial.h>                      //http://gammon.com.au/Arduino/SendOnlySoftwareSerial.zip
SendOnlySoftwareSerial Bluetooth_Serial (Bluetooth_TX);
#endif

#ifdef USE_HardwareSerial
#define Bluetooth_Serial Hardware_Bluetooth_Port
#endif

#ifdef Use_NMEA_Bluetooth_Uplink
#include "Generate_NMEA2.h"
#endif

#include "Binary2.h"

#define max_functions 5                           //number of functions in main program loop

//**************************************************************************************************
// Eventally - the program itself starts ............
//**************************************************************************************************


void loop()
{
  run_function();
}


void run_function()
{
  //following a key press to change function this routine runs the appropriate function

  delay(switch_delay);    //debounce switch a bit

  switch (Function_Number)
  {
    case 1:
      current_screen_number = 1;
      Check_LoRa_and_GPS(TrackerMode, 0);
      break;

    case 2:
      current_screen_number = 2;
      Check_LoRa_and_GPS(SearchMode, 0);
      break;

    case 3:
      current_screen_number = 3;
      //Serial.println(F("Tracker Data Display"));
      Check_LoRa_and_GPS(TrackerMode, 0);
      break;

    case 4:
      current_screen_number = 4;
      //Serial.println(F("Local Data Display"));
      Check_LoRa_and_GPS(TrackerMode, 0);
      break;

    case 5:
      current_screen_number = 5;
      Check_LoRa_and_GPS(BindMode, 0);
      break;

    default:
      break;
  }
}

void Display_Listen_Mode()
{
  if ((current_screen_number == 1) || (current_screen_number == 3) || (current_screen_number == 4))
  {
    Serial.print(F("Tracker"));
  }
  if (current_screen_number == 2)
  {
    Serial.print(F("Search"));
  }
  if (current_screen_number == 5)
  {
    Serial.print(F("Bind"));
  }

  Serial.print(F(" Listen, "));
}


void print_CurrentLoRaSettings()
{
  //prints the current LoRa settings, reads device registers
  float tempfloat;
  int tempint;
  byte regdata;
  unsigned long bw;

  tempfloat = lora_GetFreq();
  Serial.print(tempfloat, 3);
  Serial.print(F("MHz  ("));

  tempint = ramc_CalibrationOffset;
  tempfloat = (float)(tempint / 1000);
  Serial.print(tempfloat, 1);

  Serial.print(F("Khz)"));

  regdata = lora_Read(lora_RegModemConfig1);
  regdata = regdata & 0xF0;
  bw = lora_returnbandwidth(regdata);
  Serial.print(F("  BW"));
  Serial.print(bw);

  regdata = lora_Read(lora_RegModemConfig2);
  regdata = (regdata & 0xF0) / 16;
  Serial.print(F("  SF"));
  Serial.print(regdata);

  regdata = lora_Read(lora_RegModemConfig1);
  regdata = regdata & B00001110;
  regdata = regdata / 2; //move right one
  regdata = regdata + 4;

  Serial.print(F("  CR4/"));
  Serial.print(regdata);

  regdata = lora_Read(lora_RegModemConfig3);
  regdata = regdata & B00001000;
  Serial.print(F("  LDROPT_"));
  if (regdata == 8)
  {
    Serial.print(F("ON"));
  }
  else
  {
    Serial.print(F("OFF"));
  }

  regdata = lora_Read(lora_RegPaConfig);
  regdata = regdata - 0xEE;

  Serial.print(F("  Power "));
  Serial.print(regdata);
  Serial.print(F("dBm"));
  Serial.println();
}


byte Check_LoRa_and_GPS(byte lmode, unsigned long listen_mS)
{
  //listen for packets in specific mode, timeout in mS, 0 = continuous wait
  byte GPSByte;

  switch (lmode)
  {
    case TrackerMode:
      Setup_LoRaTrackerMode();
      break;

    case SearchMode:
      Setup_LoRaSearchMode();
      break;

    case BindMode:
      Setup_LoRaBindMode();
      break;
  }

  Switchpress = 0;

  print_CurrentLoRaSettings();

  update_screen(current_screen_number);

  Display_Listen_Mode();
  lora_RXpacketCount = 0;
  Switchpress = 0;
  keypress = 0;
  lora_RXONLoRa();

  do
  {

    if (!digitalRead(SWITCH1))
    {
      Serial.println(F("Switch1"));
      Serial.println();
      Switchpress = SWITCH1;
      Function_Number++;
    }

    if (Function_Number > max_functions)
    {
      Function_Number = 1;
    }

    if (Function_Number == 0)
    {
      Function_Number = max_functions;
    }

    check_for_Packet();

#ifndef Do_Not_Use_GPS
    do
    {
      GPSByte = GPS_GetByte();
      if (GPSByte != 0xFF)
      {
        gps.encode(GPSByte);
      }
    }
    while (GPSByte != 0xFF);

    check_GPSforFix();
#endif


#ifdef Do_Not_Use_GPS
   
    Local_GPS_Fix = true;
    GLONASS_Active = false;

    record_LocalData();
    save_LocalData();
    
    distanceto();
    directionto();

    update_screen(current_screen_number);
    delay(2000);

#endif

  }
  while ((Switchpress == 0));

  return keypress;
}


byte check_for_Packet()
{
  //checks to see if a packet has arrived
  byte returnbyte;

#ifdef No_DIO0_RX_Ready
  returnbyte = lora_readRXready();
#else
  returnbyte = lora_readRXready2();
#endif

  if (returnbyte == 64)
  {
    digitalWrite(LED1, HIGH);
    lora_ReadPacket();
    process_Packet();
    digitalWrite(LED1, LOW);
    Display_Listen_Mode();
    lora_RXONLoRa();                                       //ready for next and clear flags
    return 1;

  }

  if (returnbyte == 96)
  {
    Serial.println(F("CRC Error"));
    Display_Listen_Mode();
    lora_RXONLoRa();    //ready for next
  }
  return 0;                                                //no valid packet received
}


void write_HABPacketMemory(byte RXStart, byte RXEnd)
{
  //stores the HAB packet in memory, FRAM or EEPROM
  byte index, bufferdata;
  unsigned int MemAddr = addr_StartHABPayloadData;         //address in FRAM where last received HAB payload stored

  Memory_WriteByte(MemAddr, lora_RXPacketType);
  MemAddr++;
  Memory_WriteByte(MemAddr, lora_RXDestination);
  MemAddr++;
  Memory_WriteByte(MemAddr, lora_RXSource);
  MemAddr++;
  for (index = RXStart; index <= RXEnd; index++)
  {
    bufferdata = lora_RXBUFF[index];
    Memory_WriteByte(MemAddr, bufferdata);
    MemAddr++;
  }
  Memory_WriteByte(MemAddr, 0xFF);

}


void extract_HABPacket(byte passedRXStart, byte passedRXEnd)
{
  //extracts data from received HAB packets where first fields are lat,lon,alt.
  byte tempbyte;
  byte savedRXStart;

  savedRXStart = lora_RXStart;                            //save current value of lora_RXStart
  lora_RXStart = passedRXStart;                           //use lora_RXStart as its global

  //Skip leading $
  do
  {
    tempbyte =  lora_RXBUFF[lora_RXStart++];
  }
  while ( tempbyte == '$');
  lora_RXStart--;

  lora_RXStart = next_Comma(lora_RXStart);
  lora_RXStart = next_Comma(lora_RXStart);
  lora_RXStart = next_Comma(lora_RXStart);

  //Lat
  TRLat = extract_Float();

  //Lon
  TRLon = extract_Float();

  //Alt
  TRAlt = extract_Uint();

  lora_RXStart = next_Comma(lora_RXStart);
  lora_RXStart = next_Comma(lora_RXStart);

  lora_RXStart = savedRXStart;                              //restore lora_RXStart, just in case
}


float extract_Float()
{
  //extracts a float in ASCII format from buffer
  char temp[12];
  byte tempptr = 0;
  byte bufferdata;
  float tempfloat;
  do
  {
    bufferdata =  lora_RXBUFF[lora_RXStart++];
    temp[tempptr++] = bufferdata;
  }
  while (bufferdata != ',');
  temp[tempptr] = 0;  //terminator for string
  tempfloat = (float)atof(temp);
  return tempfloat;
}


float extract_Uint()
{
  //extracts an unsigned int in ASCII format from buffer
  char temp[12];
  byte tempptr = 0;
  byte buffdata;
  unsigned int tempint;
  do
  {
    buffdata =  lora_RXBUFF[lora_RXStart++];
    temp[tempptr++] = buffdata;
  }
  while (buffdata != ',');
  temp[tempptr] = 0;  //terminator for string
  tempint = (unsigned int)atof(temp);
  return tempint;
}


byte next_Comma(byte localpointer)
{
  //skips through HAB packet (in CSV format) to next  comma
  byte bufferdata;
  do
  {
    bufferdata =  lora_RXBUFF[localpointer++];
  }
  while (bufferdata != ',');
  return localpointer;
}


/**********************************************************************
  Tracker data routines
***********************************************************************
*/

void extract_TRbinarylocationData()
{
  //extracts the binary location data from receive buffer and records date and time
  TRLat = Read_Float(0, lora_RXBUFF);
  TRLon = Read_Float(4, lora_RXBUFF);
  TRAlt = Read_UInt(8, lora_RXBUFF);
}


void save_TRData()
{
  //writes the last received tracker location data to memory
  Memory_WriteFloat(addr_TRLat, TRLat);
  Memory_WriteFloat(addr_TRLon, TRLon);
  Memory_WriteUInt(addr_TRAlt, TRAlt);
}


void read_TRData()
{
  //read stored tracker location data from memory
  TRLat = Memory_ReadFloat(addr_TRLat);
  TRLon = Memory_ReadFloat(addr_TRLon);
  TRAlt = Memory_ReadUInt(addr_TRAlt);
}


void clear_TRData()
{
  //sets the tracker location data to zero
  TRLat = 0;
  TRLon = 0;
  TRAlt = 0;
}


/**********************************************************************
  Local data routines
***********************************************************************
*/

void record_LocalData()
{
  //gets the current local location fix data and time
  //time_t t = now(); // Store the current time in time
  LocalLat = gps.location.lat();
  LocalLon = gps.location.lng();
  LocalAlt = gps.altitude.meters();

#ifdef Use_Test_Location                                    //overwrite local GPS location for test purposes
  LocalLat = TestLatitude;
  LocalLon = TestLongitude;
  LocalAlt = TestAltitude;
#endif

}


void save_LocalData()
{
  //writes local GPS location data to memory
  Memory_WriteFloat(addr_LocalLat, LocalLat);              //save tracker lat in non volatile memory
  Memory_WriteFloat(addr_LocalLon, LocalLon);              //save tracker lon in non volatile memory
  Memory_WriteUInt(addr_LocalAlt, LocalAlt);               //save tracker lon in non volatile memory
}



void read_LocalData()
{
  //reads local GPS location data from memory
  LocalLat = Memory_ReadFloat(addr_LocalLat);
  LocalLon = Memory_ReadFloat(addr_LocalLon);
  LocalAlt = Memory_ReadUInt(addr_LocalAlt);
}


void clear_LocalData()
{
  //sets the current local location data to zero
  LocalLat = 0;
  LocalLon = 0;
  LocalAlt = 0;
}


boolean Is_Key_Valid()
{
  //checks if protection key in RX buffer matches

  //Serial.print(F("Received Key "));
  //Serial.write(lora_RXBUFF[0]);
  //Serial.write(lora_RXBUFF[1]);
  //Serial.write(lora_RXBUFF[2]);
  //Serial.write(lora_RXBUFF[3]);
  //Serial.println();

  if ( (lora_RXBUFF[0] == key0) && (lora_RXBUFF[1] == key1)  && (lora_RXBUFF[2] == key2)  && (lora_RXBUFF[3] == key3) )
  {
    //Serial.println(F("Key Valid"));
    return true;
  }
  else
  {
    Serial.println(F("Key Not Valid"));
    return false;
  }
}


void process_Packet()
{
  //process and decide what to do with received packet
  unsigned int index, tempint;
  byte tempbyte, ptr;
  unsigned int returnedCRC;

  digitalWrite(LED1, HIGH);

  if (lora_RXPacketType == LocationBinaryPacket)
  {
    Remote_GPS_Fix = true;

    if (modenumber == SearchMode)
    {
      SearchMode_Packets++;
    }

    if (modenumber == TrackerMode)
    {
      TrackerMode_Packets++;
    }

    extract_TRbinarylocationData();
    save_TRData();
    print_Tracker_Location();
    Serial.println();
    TRStatus = Read_Byte(10, lora_RXBUFF);


    if (Local_GPS_Fix)
    {
      distanceto();
      directionto();
    }

    update_screen(current_screen_number);              //and update the appropriate screen
    digitalWrite(LED1, LOW);

#ifdef Use_NMEA_Bluetooth_Uplink
    send_NMEA(TRLat, TRLon, TRAlt);                    //Send position to Bluetooth, sends two NMEA strings
    Serial.println();
#endif
    return;
  }


  if (lora_RXPacketType == HABPacket)
  {
    Remote_GPS_Fix = true;
    TrackerMode_Packets++;

    lora_RXBuffPrint(PrintASCII);                      //print packet contents as ASCII
    Serial.println();

    write_HABPacketMemory(lora_RXStart, lora_RXEnd);
    extract_HABPacket(lora_RXStart, lora_RXEnd);

    save_TRData();

    if (Local_GPS_Fix)
    {
      distanceto();
      directionto();
    }

    update_screen(current_screen_number);               //and update the appropriate screen
    digitalWrite(LED1, LOW);

#ifdef Use_NMEA_Bluetooth_Uplink
    send_NMEA(TRLat, TRLon, TRAlt);                     //Send position to Bluetooth
    Serial.println();
#endif

    return;
  }


  if (lora_RXPacketType == Bind )
  {

    Serial.println(F("Tracker Bind Received"));
    ptr = 4;                                           //set pointer to start of Bind data in lora_RXBUFF

    if (!Is_Key_Valid())
    {
      return;
    }

    if ((Function_Number != 5))                        //only accept incoming bind request when in function 5
    {
      Serial.println(F("Not in Bind Mode"));
      return;
    }

    Print_CRC_Bind_Memory();

    tempint = (lora_RXBUFF[lora_RXEnd] * 256) + (lora_RXBUFF[lora_RXEnd - 1]);

    Serial.print(F("Transmitted CRC "));
    Serial.println(tempint, HEX);
    returnedCRC = RXBuffer_CRC(ptr, (lora_RXEnd - 2));
    Serial.print(F("Received CRC "));
    Serial.println(returnedCRC, HEX);


    if (returnedCRC == tempint)
    {
      Serial.println(F("Accepted"));
      writescreen_Alert2();

      for (index = addr_StartBindData; index <= addr_EndBindData; index++)
      {
        tempbyte = lora_RXBUFF[ptr++];
        Memory_WriteByte(index, tempbyte);
      }
      read_Settings_Memory();                          //now bring the new settings into use
      Print_CRC_Bind_Memory();
    }
    else
    {
      Serial.println(F("Rejected"));
      writescreen_Alert7();
      return;
    }
  }


  if (lora_RXPacketType == NoFix)
  {
    writescreen_Alert4();
    return;
  }

  if (lora_RXPacketType == NoGPS)
  {
    writescreen_Alert4();
    return;
  }

  if (lora_RXPacketType == PowerUp)
  {
    tempint = Read_UInt(2, lora_RXBUFF);
    writescreen_Alert5(tempint);
    return;
  }


  if (lora_RXPacketType == Testpacket)
  {
    Serial.print(F("TestPacket "));
    lora_RXBuffPrint(PrintASCII);                        //print packet contents as ASCII
    Serial.println();
    writescreen_7();

    return;
  }

  if (lora_RXPacketType == IsLost)
  {
    Serial.print(F("Tracker Lost"));
    lora_RXBuffPrint(PrintASCII);                        //print packet contents as ASCII
    Serial.println();
    writescreen_Alert10();
    delay(1500);
    return;
  }


  Serial.println(F("Packet not recognised"));

}


void Print_CRC_Bind_Memory()
{
  unsigned int returnedCRC = Memory_CRC(addr_StartBindData, addr_EndBindData);
  Serial.print(F("Local Bind CRC "));
  Serial.println(returnedCRC, HEX);
}


unsigned int RXBuffer_CRC(unsigned int startaddr, unsigned int endaddr)
{
  unsigned int i, CRC;

  CRC = 0xffff;                                              //start value for CRC16
  byte j;

  for (i = startaddr; i <= endaddr; i++)                     //element 4 is first character after $$$$ at start
  {
    CRC ^= ((uint16_t) lora_RXBUFF[i] << 8);
    for (j = 0; j < 8; j++)
    {
      if (CRC & 0x8000)
        CRC = (CRC << 1) ^ 0x1021;
      else
        CRC <<= 1;
    }
  }
  return CRC;

}


void print_Tracker_Location()
{
  //prints the tracker location data only
  Serial.print(TRLat, 6);
  Serial.print(F(","));
  Serial.print(TRLon, 6);
  Serial.print(F(","));
  Serial.print(TRAlt);
}


void led_Flash(unsigned int flashes, unsigned int dealymS)
{
  //flash LED to show tracker is alive
  unsigned int index;

  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    delay(dealymS);
    digitalWrite(LED1, LOW);
    delay(dealymS);
  }
}


void read_Settings_Defaults()
{
  //To ensure the program routines are as common as possible betweeen transmitter and receiver
  //this receiver program uses constants in RAM copied from memory in the same way as the transmitter.
  //There are some exceptions, where the local programs need to use a setting unique to the particular
  //receiver.
  //Serial.print(F("Configuring Settings from Defaults - "));
  ramc_CalibrationOffset = CalibrationOffset;
  ramc_TrackerMode_Frequency = TrackerMode_Frequency;
  ramc_SearchMode_Frequency = SearchMode_Frequency;
  ramc_TrackerMode_Bandwidth = TrackerMode_Bandwidth;
  ramc_TrackerMode_SpreadingFactor = TrackerMode_SpreadingFactor;
  ramc_TrackerMode_CodeRate = TrackerMode_CodeRate;
  ramc_SearchMode_Bandwidth = SearchMode_Bandwidth;
  ramc_SearchMode_SpreadingFactor = SearchMode_SpreadingFactor;
  ramc_SearchMode_CodeRate = SearchMode_CodeRate;
  ramc_TrackerMode_Power = TrackerMode_Power;
  ramc_SearchMode_Power = SearchMode_Power;
}


void read_Settings_Memory()
{
  //To ensure the program routines are as common as possible betweeen transmitter and receiver
  //this receiver program uses constants in RAM copied from memory in the same way as the transmitter.
  //There are some exceptions, where the local programs need to use a setting unique to the particular
  //receiver.
  //Serial.print(F("Configuring Settings from Memory"));
  ramc_CalibrationOffset = Memory_ReadInt(addr_CalibrationOffset);
  ramc_TrackerMode_Frequency = Memory_ReadULong(addr_TrackerMode_Frequency);
  ramc_SearchMode_Frequency = Memory_ReadULong(addr_SearchMode_Frequency);
  ramc_TrackerMode_Bandwidth = Memory_ReadByte(addr_TrackerMode_Bandwidth);
  ramc_TrackerMode_SpreadingFactor = Memory_ReadByte(addr_TrackerMode_SpreadingFactor);
  ramc_TrackerMode_CodeRate = Memory_ReadByte(addr_TrackerMode_CodeRate);
  ramc_SearchMode_Bandwidth = Memory_ReadByte(addr_SearchMode_Bandwidth);
  ramc_SearchMode_SpreadingFactor = Memory_ReadByte(addr_SearchMode_SpreadingFactor);
  ramc_SearchMode_CodeRate = Memory_ReadByte(addr_SearchMode_CodeRate);
  ramc_TrackerMode_Power = Memory_ReadByte(addr_TrackerMode_Power);
  ramc_SearchMode_Power = Memory_ReadByte(addr_SearchMode_Power);
}


void write_Settings_Memory()
{
  //To ensure the program routines are as common as possible betweeen transmitter and receiver
  //this receiver program uses constants in RAM copied from memory in the same way as the transmitter.
  //There are some exceptions, where the local programs need to use a setting unique to the particular
  //receiver..
  //Serial.print(F("Writing RAM Settings to Memory"));
  Memory_Set(addr_StartConfigData, addr_EndConfigData, 0);          //clear memory area first
  Memory_WriteInt(addr_CalibrationOffset, ramc_CalibrationOffset);
  Memory_WriteULong(addr_TrackerMode_Frequency, ramc_TrackerMode_Frequency);
  Memory_WriteULong(addr_SearchMode_Frequency, ramc_SearchMode_Frequency);
  Memory_WriteByte(addr_TrackerMode_Bandwidth, ramc_TrackerMode_Bandwidth);
  Memory_WriteByte(addr_TrackerMode_SpreadingFactor, ramc_TrackerMode_SpreadingFactor);
  Memory_WriteByte(addr_TrackerMode_CodeRate, ramc_TrackerMode_CodeRate);
  Memory_WriteByte(addr_SearchMode_Bandwidth, ramc_SearchMode_Bandwidth);
  Memory_WriteByte(addr_SearchMode_SpreadingFactor, ramc_SearchMode_SpreadingFactor);
  Memory_WriteByte(addr_SearchMode_CodeRate, ramc_SearchMode_CodeRate);
  Memory_WriteByte(addr_TrackerMode_Power, ramc_TrackerMode_Power);
  Memory_WriteByte(addr_SearchMode_Power, ramc_SearchMode_Power);
}


//*******************************************************************************************************
// Memory Routines
//*******************************************************************************************************


void Clear_All_Memory()
{
  //clears the whole of memory, normally 1kbyte
  Serial.print(F("Clear Memory"));
  Memory_Set(addr_StartMemory, addr_EndMemory, 0);
}


void check_GPSforFix()
{
  //check GPS for new fix
  if (gps.location.isUpdated() && gps.altitude.isUpdated())          //check the fix is updated
  {
    //last_LocalGPSfixmS = millis();

    Local_GPS_Fix = true;
    GLONASS_Active = false;

    record_LocalData();
    save_LocalData();
    
    distanceto();
    directionto();

    update_screen(current_screen_number);

  }
  else
  {

#ifndef Do_Not_Use_GPS
    if (GNGGAFIXQ.age() < 2000)                     //check to see if GLONASS has gone active
    {
      Serial.println(F("GLONASS !"));
      GLONASS_Active = true;
      GPS_Setup();
      writescreen_Alert1();
      delay(1000);
      update_screen(current_screen_number);
    }
    else
    {
      GLONASS_Active = false;
    }

    if (gps.location.age() > FixisoldmS)
    {
      Local_GPS_Fix = false;
    }
#endif

  }

}


void directionto()
{
  //using remote and local lat and long calculate direction in degrees
  TRdirection = (int) TinyGPSPlus::courseTo(LocalLat, LocalLon, TRLat, TRLon);
}


void distanceto()
{
  //using remote and local lat and long calculate distance in metres
  TRdistance = TinyGPSPlus::distanceBetween(LocalLat, LocalLon, TRLat, TRLon);
}


void Setup_LoRaTrackerMode()
{
  //sets LoRa modem to Tracker mode
  lora_SetFreq(ramc_TrackerMode_Frequency, ramc_CalibrationOffset);
  lora_SetModem2(ramc_TrackerMode_Bandwidth, ramc_TrackerMode_SpreadingFactor, ramc_TrackerMode_CodeRate, Explicit);  //Setup the LoRa modem parameters
  lora_Power = ramc_TrackerMode_Power;
  modenumber = TrackerMode;
}


void Setup_LoRaSearchMode()
{
  //sets LoRa modem to Search mode
  lora_SetFreq(ramc_SearchMode_Frequency, ramc_CalibrationOffset);
  lora_SetModem2(ramc_SearchMode_Bandwidth, ramc_SearchMode_SpreadingFactor, ramc_SearchMode_CodeRate, Explicit);  //Setup the LoRa modem parameters
  lora_Power = ramc_SearchMode_Power;
  modenumber = SearchMode;
}


void Setup_LoRaBindMode()
{
  //sets LoRa modem to Bind mode
  lora_SetFreq(BindMode_Frequency, ramc_CalibrationOffset);
  lora_SetModem2(BindMode_Bandwidth, BindMode_SpreadingFactor, BindMode_CodeRate, Explicit); //Setup the LoRa modem parameters
  lora_Power = BindMode_Power;
  modenumber = BindMode;
}


unsigned int ReadSupplyVolts()
{
  //relies on 1V1 internal reference and 91K & 11K resistor divider
  //returns supply in mV @ 10mV per AD bit read
  unsigned int temp;
  byte i;
  SupplyVolts = 0;

  analogReference(INTERNAL);
  for (i = 0; i <= 2; i++)                      //sample AD 3 times
  {
    temp = analogRead(SupplyAD);
    SupplyVolts = SupplyVolts + temp;
  }
  SupplyVolts = (unsigned int) ((SupplyVolts / 3) * ADMultiplier);
  return SupplyVolts;
}


void setup()
{
  //needs no explanation I hope ...............

  pinMode(LED1, OUTPUT);                    //setup pin for PCB LED
  led_Flash(2, 250);

  pinMode(GPSPOWER, OUTPUT);                //setup pin for GPSPower contol, if fitted
  digitalWrite(GPSPOWER, LOW);

  Serial.begin(9600);                       //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(aurthorname));
  ReadSupplyVolts();

  pinMode(SWITCH1, INPUT_PULLUP);            //setup switches

#ifndef Do_Not_Use_GPS
  GPS_On(DoGPSPowerSwitch);
#endif

  SPI.begin();                               //initialize SPI:
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  pinMode(lora_NReset, OUTPUT);              //LoRa Device reset line
  pinMode (lora_NSS, OUTPUT);                //LoRa Device select line
  pinMode (lora_DIO0, INPUT);                //LoRa DIO0 pin, used for RX ready
  
  digitalWrite(lora_NSS, HIGH);
  digitalWrite(lora_NReset, HIGH);
  delay(1);

  Memory_Start();
  Display_Setup();

#ifdef ClearAllMemory
  writescreen_9();
  Clear_All_Memory();
#endif


#ifdef ConfigureDefaults
  read_Settings_Defaults();
  write_Settings_Memory();
  Serial.println();
#endif

#ifdef ConfigureFromMemory
  read_Settings_Memory();
#endif


#ifdef write_CalibrationOffset
  Memory_WriteInt(addr_CalibrationOffset, CalibrationOffset);
#endif

  read_TRData();
  read_LocalData();

  if (!lora_CheckDevice())
  {
    Serial.println(F("LoRa Error"));
    led_Flash(40, 50);
  }

  lora_Setup();

#ifdef Use_NMEA_Bluetooth_Uplink
  Bluetooth_Serial_Setup();
#endif

  Setup_LoRaTrackerMode();

  writescreen_8(ramc_TrackerMode_Frequency, ramc_SearchMode_Frequency, SupplyVolts);

#ifdef CalibrateTone
  digitalWrite(LED1, HIGH);                   //turn on LED
  lora_Tone(1000, 3000, 10);                  //Transmit an FM tone, 1000hz, 1000ms, 10dBm
  digitalWrite(LED1, LOW);                    //LED is off
  delay(1000);
#endif

  update_screen(3);

  lora_RXONLoRa();

#ifndef Do_Not_Use_GPS
  GPS_On(DoGPSPowerSwitch);
  GPS_Setup();
  Serial.println();
#endif

 
#ifdef Use_NMEA_Bluetooth_Uplink
  Bluetooth_Serial.println("Bluetooth Active");
#endif

  Function_Number = 1;                           //start in tracker mode

}


