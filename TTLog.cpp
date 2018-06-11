#include <Arduino.h>
#include "TT_Log.h"

#include <SPI.h>
#include <SD.h>
#include <Time.h>
#include <TimeLib.h>

TT_Log::TT_Log() {}

void TT_Log::begin(uint8_t sd_cs_pin)
{
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(sd_cs_pin))
  {
    Serial.println("SD Card failed, or not present");
    // don't do anything more:
  }
  else
  {
    Serial.println("card initialized.");
  }
}

/*
  @Description
    Creates a log entry that is optionally written to the serial monitor or to
    the SD card (in CSV format)

  @param message - (const char*) to be written into the log entry
  @param filename - (String) The file name on the SD card to which to append the entry
  @param printSerial - (Bool) Whether the entry should be written to the serial monitor
  @param printSDCard - (Bool) Whether the entry should be written to the SD card
*/
void TT_Log::entryCSV(String &message_str, String &filename,
                     bool print_serial, bool print_sd_card)
{
  appendDateTimeCSV(message_str);
  print(message_str, filename, print_serial, print_sd_card);
}

void TT_Log::entryCSV(const char *message, String &filename,
                  bool print_serial, bool print_sd_card)
{
  String message_str = String(message);
  entryCSV(message_str, filename, print_serial, print_sd_card);
}

/*
  @Description
    Creates a log entry that is optionally written to the serial monitor or to
    the SD card

  @param message - (const char*) to be written into the log entry
  @param filename - (String) The file name on the SD card to which to append the entry
  @param printSerial - (Bool) Whether the entry should be written to the serial monitor
  @param printSDCard - (Bool) Whether the entry should be written to the SD card
*/
void TT_Log::entryTXT(String &message_str, String &filename,
                     bool print_serial, bool print_sd_card)
{
  message_str += "\n";
  appendDateTimeTXT(message_str);
  print(message_str, filename, print_serial, print_sd_card);
}

void TT_Log::entryTXT(const char *message, String &filename,
                     bool print_serial, bool print_sd_card)
{
  String message_str = String(message);
  entryTXT(message_str, filename, print_serial, print_sd_card);
}

void TT_Log::entryHeaderCSV(String &header, String &filename,
                           bool print_serial, bool print_sd_card)
{
  print(header, filename, print_serial, print_sd_card);
}

void TT_Log::print(String &message, String &filename,
           bool print_serial, bool print_sd_card)
{
  if(print_sd_card)
  {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File DataFile = SD.open(filename, FILE_WRITE);

    // if the file is available, write to it if...
    if (DataFile)
    {
      // an intruder has entered or left...
      if(message != NO_DATA && message != "")
      {
        DataFile.println(message);
      }
      // otherwise just close the file
      DataFile.close();
    }

    // if the file isn't open, pop up an error:
    else
    {
      Serial.println("error opening " + String(filename));
    }
  }
  if(print_serial)
  {
    Serial.println(message);
  }
}

void TT_Log::setDateTime(int hour, int minute,
                        int day, int month, int year)
{
  // user entered time as params
  if(hour != -1 && minute != -1 && day != -1 && month != -1 && year != -1)
  {
    int second = 0;
    setTime(hour, minute, second, day, month, year);
    return;
  }

  // otherwise set time through Serial monitor
  // wait for Serial port to connect
  if(!Serial)
  {
    delay(300);
  }
  if(!Serial)
  {
    return;
  }

  char input_buffer[5];
  Serial.println("Please set the time");
  Serial.println();

  memset(input_buffer, '\0', sizeof(input_buffer));
  Serial.print("Hour: ");
  while(!Serial.available());
  int i = 0;
  while(Serial.available())
  {
    delay(10);
    input_buffer[i] = Serial.read();
    i++;
    if(i > 1)
    {
      break;
    }
  }
  hour = atoi(input_buffer);
  Serial.println(hour);

  memset(input_buffer, '\0', sizeof(input_buffer));
  Serial.print("Minute: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available())
  {
    delay(10);
    input_buffer[i] = Serial.read();
    i++;
    if(i > 1)
    {
      break;
    }
  }
  minute = atoi(input_buffer);
  Serial.println(minute);

  memset(input_buffer, '\0', sizeof(input_buffer));
  Serial.print("Month: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available())
  {
    delay(10);
    input_buffer[i] = Serial.read();
    i++;
    if(i > 1)
    {
      break;
    }
  }
  month = atoi(input_buffer);
  Serial.println(month);

  memset(input_buffer, '\0', sizeof(input_buffer));
  Serial.print("Day: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available())
  {
    delay(10);
    input_buffer[i] = Serial.read();
    i++;
    if(i > 1)
    {
      break;
    }
  }
  day = atoi(input_buffer);
  Serial.println(day);

  memset(input_buffer, '\0', sizeof(input_buffer));
  Serial.print("Year: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available())
  {
    delay(10);
    input_buffer[i] = Serial.read();
    i++;
    if(i > 3)
    {
      break;
    }
  }

  year = atoi(input_buffer);
  Serial.println(year);
  int second = 0;

  setTime(hour, minute, second, day, month, year);

  String sNow;
  appendDateTimeTXT(sNow);
  Serial.println("It is now: ");
  Serial.println(sNow);
}


void TT_Log::appendDateTimeTXT(String &sDateTime)
{
  appendTimeTXT(sDateTime);
  appendDateTXT(sDateTime);
}

void TT_Log::appendTimeTXT(String &time_str)
{
  String hour_str; String minute_str;
  time_t t = now();

  if(hour(t) < 10)
  {
    hour_str = "0" + String(hour(t));
  }
  else
  {
    hour_str = String(hour(t));
  }

  if(minute(t) < 10)
  {
    minute_str = "0" + String(minute(t));
  }
  else
  {
    minute_str = String(minute(t));
  }

  time_str += hour_str + ":" + minute_str + "\n";
}

void TT_Log::appendDateTXT(String &date_str)
{
  time_t t = now();
  String month_str = String(monthShortStr(month()));
  date_str += month_str + " ";
  date_str += String(day(t)) + " ";
  date_str += String(year(t)) + "\n";
}

void TT_Log::appendDateTimeCSV(String &sDateTime)
{
  appendTimeCSV(sDateTime);
  appendDateCSV(sDateTime);
}

void TT_Log::appendDateCSV(String &date_str)
{
  time_t t = now();
  String month_str = String(monthShortStr(month()));
  date_str += month_str + ",";
  date_str += String(day(t)) + ",";
  date_str += String(year(t));
}

void TT_Log::appendTimeCSV(String &time_str)
{
  String hour_str; String minute_str;
  time_t t = now();

  if(hour(t) < 10)
  {
    hour_str = "0" + String(hour(t));
  }
  else
  {
    hour_str = String(hour(t));
  }

  if(minute(t) < 10)
  {
    minute_str = "0" + String(minute(t));
  }
  else
  {
    minute_str = String(minute(t));
  }

  time_str += hour_str + ":" + minute_str + ",";
}
