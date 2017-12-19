//Portable_Screens_Small.h
/*
**************************************************************************************************

  Easy Build LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson - 05/08/2017

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

  These screens are for a ILI9341 320 x 240 display, in text size 3 this is 18 columnd x 10 rows

  To Do:

**************************************************************************************************
*/


void writescreen_1();             //tracker mode receive screen
void writescreen_2();             //search mode receive screen
void writescreen_3();             //tracker location screen
void writescreen_4();             //local location screen
void writescreen_5();             //send bind screen
void writescreen_6();             //receive bind screen
void writescreen_7();             //test packet screen
void writescreen_8(uint32_t f1, uint32_t f2, int volts);   //frequencies screen
void writescreen_9();             //memory clear 
void writescreen_Alert1();        //GLONASS warning
void writescreen_Alert2();        //bind received
void writescreen_Alert4();        //no tracker GPS fix
void writescreen_Alert5(unsigned int battvolts);
void writescreen_Alert7();        //bind rejected 
void writescreen_Alert8();        //memory clear
void writescreen_Alert9();        //tracker GPS error
void writescreen_Alert10();       //tracker lost 
void Display_Setup();             //needed by some programs

void writeSNR(byte col, byte row);
void revert_Screen();
void add_Tracker_Location();
void add_Tracker_DD();

unsigned int writePktCount(byte col, byte row, unsigned int packetcount);
byte current_screen_number = 1;

#define Display_Address 0x08
I2CDisplay disp(Display_Address);                  //create the display class

/******************************************************
  Screens
*******************************************************/


void update_screen(byte screennumber)
{
  switch (screennumber)
  {
    case 1:
      writescreen_1();
      break;

    case 2:
      writescreen_2();
      break;

    case 3:
      writescreen_3();
      break;

    case 4:
      writescreen_4();
      break;

    case 5:
      writescreen_5();
      break;

    default:
      break;
  }
}


void writescreen_1()
{
  disp.settextsize(27);
  //disp.clear();
  add_Tracker_Location();
 
 if (Local_GPS_Fix)
 {
  disp.setcurpos(0, 3);
  add_Tracker_DD();
 }
 
  writeSNR(0, 4);
  disp.setcurpos(0, 8);
  disp.print(F("Tracker Pkts "));
  disp.print(TrackerMode_Packets);
  disp.print(F("     "));
  //disp.update();
}



void writescreen_2()
{
  disp.settextsize(27);
  //disp.clear();
  add_Tracker_Location();
 
  if (Local_GPS_Fix)
  {
   disp.setcurpos(0, 3);
   add_Tracker_DD();
  }

  writeSNR(0, 4);
  disp.setcurpos(0, 8);
  disp.print(F("Search Pkts "));
  disp.print(SearchMode_Packets);
  disp.print(F("     "));
  //disp.update();
}


void writescreen_3()
{
  //This displays last received tracker co-ordinates stored in memory;
  disp.settextsize(27);
  //disp.clear();
  add_Tracker_Location();
  disp.setcurpos(0, 8);
  disp.print(F("Last Tracker Fix"));
  //disp.update();
}


void writescreen_4()
{
  //This displays last Local co-ordinates from memory;
  disp.settextsize(27);
  //disp.clear();
  disp.setcurpos(0, 0);
  disp.print(F("LAT "));
  disp.print(LocalLat, 6);
  disp.setcurpos(0, 1);
  disp.print(F("LON "));
  disp.print(LocalLon, 6);
  disp.setcurpos(0, 2);
  disp.print(F("Alt "));
  disp.print(LocalAlt);
  disp.setcurpos(0, 8);
  disp.print(F("Last Local Fix  "));

  //disp.update();
}



void writescreen_5()
{
  disp.settextsize(27);
  disp.clear();
  disp.setcurpos(0, 0);
  disp.print(F("Receive Bind"));
  //disp.update();
}


void writescreen_7()
{
  byte tempbyte, tempbyte1, power;
  //disp.clear();
  disp.setcurpos(0, 0);
  disp.print("Test Packet    ");
  disp.setcurpos(0, 1);
  disp.print("Power    ");
  tempbyte = lora_RXBUFF[0] - 48;                    //convert ASCII to number
  tempbyte1 = lora_RXBUFF[1] - 48;                   //convert ASCII to number
  power = (tempbyte * 10) + tempbyte1;
  disp.print(power);
  disp.print("dBm");
  writeSNR(0, 4);
  delay(750);                                         //so we have enough time to see packet on screen
  disp.clear();
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
void writescreen_8(uint32_t f1, uint32_t f2, int volts)
{
  //float tempfloat;
  disp.clear();
  disp.setcurpos(0, 0);
  disp.print(F("Locator2"));
  disp.setcurpos(0, 2);
  disp.print(F("Tracker "));
  disp.print(f1);
  disp.setcurpos(0, 3);
  disp.print(F("Search  "));
  disp.print(f2);
  disp.setcurpos(0, 5);
  disp.print(volts);
  disp.print(F("mV"));
  delay(750);                                         //so we have enough time to see packet on screen
  disp.clear();
}


void writescreen_9()
{
  //float tempfloat;
  disp.clear();
  disp.setcurpos(0, 0);
  disp.println(F("Memory Clear"));
  disp.println(F("Press SWITCH1"));
  ////disp.update();
}

void add_Tracker_DD()
{
float temp;

if (TRdistance < 1000)
  {
    disp.print(TRdistance, 0);
    disp.print(F("M"));
  }
  else if (TRdistance < 10000)
  {
    temp = (float)(TRdistance / 1000);
    disp.print(temp, 2);
    disp.print(("Km"));
  }
  else if (TRdistance < 100000)
  {
    temp = (float)(TRdistance / 1000);
    disp.print(temp, 1);
    disp.print(("Km"));
  }
  else
  {
    temp = (float)(TRdistance / 1000);
    disp.print(temp, 0);
    disp.print(("Km"));
  }

  disp.print(F("  "));
  disp.print(TRdirection);
  disp.print(F("D"));
}


void add_Tracker_Location()
{
  disp.setcurpos(0, 5);
  
  if (Local_GPS_Fix)
  {
  disp.print(F("G"));
  }
  else
  {
  disp.print(F("?"));  
  }
    
  disp.setcurpos(0, 0);
  disp.print(F("LAT "));
  disp.print(TRLat, 6);
  disp.print(F("  "));
  disp.setcurpos(0, 1);
  disp.print(F("LON "));
  disp.print(TRLon, 6);
  disp.print(F("  "));
  disp.setcurpos(0, 2);
  disp.print(F("Alt "));
  disp.print(TRAlt);
  disp.print(F("M    "));
}


void writeSNR(byte col, byte row)
{
  if (lora_RXpacketCount)                       //if no packets do not display SNR
  {
    disp.settextsize(27);
    disp.setcurpos(col, row);
    disp.print(F("SNR "));
    disp.print(lora_PacketSNR);                 //now print the SNR
    disp.print(F("dB"));
  }

}


unsigned int writePktCount(byte col, byte row, unsigned int packetcount)
{
  //now print the last two digits of lora_RXpacketCount so we can see if packets are arriving

  if (packetcount > 0)                              //if no packets do not display count
  {
    if (packetcount > 999)
    {
      packetcount = 1;
    }
    disp.settextsize(27);
    disp.setcurpos(col, row);                   //cursor to last two cols of bottom line
    disp.print(packetcount);                        //send count 0-999 to LCD
  }
  return packetcount;
}


void writescreen_Alert1()
{
  disp.settextsize(27);
  disp.clear();                                    //set large font and clear
  disp.setcurpos(0, 2);
  disp.print(F("GLONASS NMEA"));
  disp.setcurpos(0, 3);
  disp.print(F("  Detected  "));
  //disp.update();
  delay(1500);
  revert_Screen();
}


void writescreen_Alert2()
{
  disp.settextsize(27);
  disp.clear();                                    //set large font and clear
  disp.setcurpos(0, 1);
  disp.print(F("     Bind"));
  disp.setcurpos(0, 3);
  disp.print(F("   Accepted"));
  //disp.update();
  delay(1500);
  revert_Screen();
}


void writescreen_Alert4()
{
  disp.settextsize(27);
  disp.clear();                                    //set large font and clear
  disp.setcurpos(0, 1);
  disp.print(F("  No Tracker"));
  disp.setcurpos(0, 3);
  disp.print(F("   GPS Fix"));
  delay(750);
  //disp.update();
}


void writescreen_Alert5(unsigned int supplyvolts)
{
  disp.settextsize(27);
  disp.clear();                                    //set large font and clear
  disp.setcurpos(0, 0);
  disp.print(F("Power Up"));
  disp.setcurpos(0, 2);
  disp.print(F("Batt "));
  disp.print(supplyvolts);
  disp.print(F("mV"));
  disp.setcurpos(0, 5);
  disp.print(F("Tracker"));
  //disp.update();
  delay(1500);
  revert_Screen();
}


void writescreen_Alert7()
{
  disp.settextsize(27);
  disp.clear();                                    //set large font and clear
  disp.setcurpos(0, 1);
  disp.print(F("    Bind"));
  disp.setcurpos(0, 3);
  disp.print(F("  Rejected"));
  //disp.update();
  delay(1500);
  revert_Screen();
}


void writescreen_Alert8()
{
  disp.settextsize(27);
  disp.clear();                                    //set large font and clear
  disp.setcurpos(0, 1);
  disp.print(F(" Memory Clear "));
  delay(1500);
}

void writescreen_Alert9()
{
  disp.settextsize(27);
  disp.clear();                                    //set large font and clear
  disp.setcurpos(0, 1);
  disp.print(F("   Tracker"));
  disp.setcurpos(0, 3);
  disp.print(F("  GPS Error"));
  //disp.update();
}


void writescreen_Alert10()
{
  disp.clear();                                    //set large font and clear
  disp.settextsize(27);
  disp.setcurpos(3, 1);
  disp.print(F("Tracker"));
  disp.setcurpos(1, 3);
  disp.print(F("Lost"));
  //disp.update();
}


void revert_Screen()
{
  if (current_screen_number == 1)
  {
    writescreen_1();
  }

  if (current_screen_number == 2)
  {
    writescreen_2();
  }
  delay(1000);   //leave screen up for a while
}

void Display_Setup()             //needed by some programs
{
//null routine not used  
}

