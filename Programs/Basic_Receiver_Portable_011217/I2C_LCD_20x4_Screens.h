//I2C_20x4_LCD_Screens.h
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
void writescreen_8(uint32_t f1, uint32_t f2, uint32_t f3, int volts);
void writescreen_9();             //memory clear 
void writescreen_Alert1();        //GLONASS warning
void writescreen_Alert2();        //bind received
void writescreen_Alert4();        //no tracker GPS fix
void writescreen_Alert5(unsigned int battvolts);
void writescreen_Alert7();        //bind rejected
void writescreen_Alert8();        //memory clear
void writescreen_Alert9();        //tracker GPS error
void writescreen_Alert10();       //tracker lost 

void writeSNR(byte col, byte row);
void revert_Screen();
void add_Tracker_Location();
void add_Tracker_DD();
void Display_Status();

void writePktCount(byte col, byte row, unsigned int packetcount);

byte current_screen_number = 1;

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
  Display_Clear();
  add_Tracker_Location();

  if (Local_GPS_Fix)
  {
    add_Tracker_DD();
  }

  Display_SetCurPos(16, 3);
  disp.print(F("TR"));
  Display_Status();
  writeSNR(0,3);
  writePktCount(12,3,lora_RXpacketCount);
}


void Display_Status()
{
  if (Local_GPS_Fix)
  {
    disp.print(F("G"));
  }
  else
  {
    disp.print(F("?"));
  }

  if (bitRead(TRStatus,TrackerLost))
  {
  disp.print(F("L")); 
  }
 
}


void writescreen_2()
{
  Display_Clear();
  add_Tracker_Location();

  if (Local_GPS_Fix)
  {
   add_Tracker_DD();
  }

  Display_SetCurPos(16, 3);
  disp.print(F("SE"));
  Display_Status();
  writeSNR(0,3);
  writePktCount(12,3,lora_RXpacketCount);
}


void writescreen_3()
{
  //This displays last received tracker co-ordinates stored in memory;
  Display_Clear();
  add_Tracker_Location();
  Display_SetCurPos(0, 3);
  disp.print(F("Last Tracker Fix"));
  Display_Update();
}


void writescreen_4()
{
  //This displays last Local co-ordinates from memory;
  Display_Clear();
  Display_SetCurPos(0, 0);
  disp.print(F("LAT "));
  disp.print(LocalLat, 6);
  Display_SetCurPos(0, 1);
  disp.print(F("LON "));
  disp.print(LocalLon, 6);
  Display_SetCurPos(0, 2);
  disp.print(F("Alt "));
  disp.print(LocalAlt);
  Display_SetCurPos(0, 3);
  disp.print(F("Last Local Fix"));

  Display_Update();
}



void writescreen_5()
{
  Display_Clear();
  Display_SetCurPos(0, 0);
  disp.print(F("Receive Bind"));
  Display_Update();
}


void writescreen_7()
{
  byte tempbyte, tempbyte1, power;
  Display_Clear();
  Display_SetCurPos(0, 0);
  disp.print("Test Packet ");
  Display_SetCurPos(0, 1);
  disp.print("Power ");
  tempbyte = lora_RXBUFF[0] - 48;                    //convert ASCII to number
  tempbyte1 = lora_RXBUFF[1] - 48;                   //convert ASCII to number
  power = (tempbyte * 10) + tempbyte1;
  disp.print(power);
  disp.print("dBm");
  writeSNR(0, 3);
  delay(750);                                         //so we have enough time to see packet on screen
  Display_Clear();
}



void writescreen_8(uint32_t f1, uint32_t f2, int volts)
{
  //float tempfloat;
  Display_Clear();
  Display_SetCurPos(0, 0);
  disp.print(F("Locator2"));
  Display_SetCurPos(0, 1);
  disp.print(F("Tracker "));
  disp.print(f1);
  Display_SetCurPos(0, 2);
  disp.print(F("Search  "));
  disp.print(f2);
  Display_SetCurPos(0, 3);
  disp.print(volts);
  disp.print(F("mV"));
  Display_Update();
}


void writescreen_9()
{
  Display_Clear();
  Display_SetCurPos(0, 0);
  disp.print(F("Memory Clear"));
  Display_Update();
  delay(1000);
}


void add_Tracker_DD()
{
  float temp;
  Display_SetCurPos(0, 2);
  if (TRdistance < 1000)
  {
    disp.print(TRdistance, 0);
    disp.print(F("M"));
  }
  else if (TRdistance < 10000)
  {
    temp = (float)(TRdistance / 1000);
    disp.print(temp, 1);
    disp.print(("K"));
  }
  else if (TRdistance < 100000)
  {
    temp = (float)(TRdistance / 1000);
    disp.print(temp, 0);
    disp.print(("Km"));
  }
  else
  {
    temp = (float)(TRdistance / 1000);
    disp.print(temp, 0);
    disp.print(("K"));
  }

  disp.print(F(" "));
  disp.print(TRdirection);
  disp.print(F("D"));
}


void add_Tracker_Location()
{
  //prints the tracker lat, long and alt
  Display_SetCurPos(0, 0);
  disp.print(F("LAT "));
  disp.print(TRLat, 5);    //5 decimal places is enough, gives location to the nearest approx 10M
  Display_SetCurPos(0, 1);
  disp.print(F("LON "));
  disp.print(TRLon, 5);
  Display_SetCurPos(14, 0);
  disp.print(TRAlt);
  disp.print("M ");
}



void writeSNR(byte col, byte row)
{
  if (lora_RXpacketCount)                       //if no packets do not display SNR
  {

    Display_SetCurPos(col, row);
    disp.print(F("SNR "));
    disp.print(lora_PacketSNR);                 //now print the SNR
    disp.print(F("dB"));
  }

}


void writePktCount(byte col, byte row, unsigned int packetcount)
{
    Display_SetCurPos(col, row);                   //cursor to last position
    disp.print(packetcount);                       //send count to LCD
}


void writescreen_Alert1()
{
  //Display_SetTextSize(1);
  Display_Clear();                                    //set large font and clear
  Display_SetCurPos(0, 2);
  disp.print(F("GLONASS NMEA"));
  Display_SetCurPos(0, 3);
  disp.print(F("  Detected  "));
  Display_Update();
  delay(1500);
  revert_Screen();
}


void writescreen_Alert2()
{
  //Display_SetTextSize(1);
  Display_Clear();                                    //set large font and clear
  Display_SetCurPos(0, 1);
  disp.print(F("     Bind"));
  Display_SetCurPos(0, 3);
  disp.print(F("   Accepted"));
  Display_Update();
  delay(1500);
  revert_Screen();
}


void writescreen_Alert4()
{
  //Display_SetTextSize(1);
  Display_Clear();                                    //set large font and clear
  Display_SetCurPos(5, 1);
  disp.print(F("No Tracker"));
  Display_SetCurPos(5, 2);
  disp.print(F("GPS Fix"));
  delay(750);
  Display_Update();
}


void writescreen_Alert5(unsigned int supplyvolts)
{
  //Display_SetTextSize(1);
  Display_Clear();                                    //set large font and clear
  Display_SetCurPos(0, 0);
  disp.print(F("Power Up"));
  Display_SetCurPos(0, 2);
  disp.print(F("Batt "));
  disp.print(supplyvolts);
  disp.print(F("mV"));
  Display_SetCurPos(0, 3);
  disp.print(F("Tracker"));
  Display_Update();
  delay(1500);
  revert_Screen();
}


void writescreen_Alert7()
{
  //Display_SetTextSize(1);
  Display_Clear();                                    //set large font and clear
  Display_SetCurPos(0, 1);
  disp.print(F("    Bind"));
  Display_SetCurPos(0, 3);
  disp.print(F("  Rejected"));
  Display_Update();
  delay(1500);
  revert_Screen();
}


void writescreen_Alert8()
{
  //Display_SetTextSize(1);
  Display_Clear();                                    //set large font and clear
  Display_SetCurPos(0, 1);
  disp.print(F(" Memory Clear "));
  delay(1500);
}


void writescreen_Alert9()
{
  //Display_SetTextSize(1);
  Display_Clear();                                    //set large font and clear
  Display_SetCurPos(0, 1);
  disp.print(F("   Tracker"));
  Display_SetCurPos(0, 3);
  disp.print(F("  GPS Error"));
  Display_Update();
}


void writescreen_Alert10()
{
  Display_Clear();                                    //set large font and clear
  Display_SetCurPos(7, 1);
  disp.print(F("Tracker"));
  Display_SetCurPos(7, 2);
  disp.print(F("Lost"));
  Display_Update();
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

