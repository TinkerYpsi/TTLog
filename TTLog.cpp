#include <Arduino.h>
#include "TTLog.h"

#include <SPI.h>
#include <SD.h>
#include <Time.h>
#include <TimeLib.h>

TTLog::TTLog() {}

void TTLog::begin(uint8_t sd_cs_pin)
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
void TTLog::entryCSV(const char *message, String &filename,
                     bool print_serial = true, bool print_sd_card = true)
{
  String message_str = String(message);
  message_str += ",";

  appendDateTimeCSV(message_str);

  if(print_sd_card)
  {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File DataFile = SD.open(filename, FILE_WRITE);

    // if the file is available, write to it if...
    if (DataFile)
    {
      // an intruder has entered or left...
      if(message_str != NO_DATA && message_str != "")
      {
        DataFile.println(message_str);
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
    Serial.println(message_str);
    Serial.println(); // extra line is here for clarity
  }
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
void TTLog::entry(const char *message, String &filename,
                  bool print_serial, bool print_sd_card)
{
  String message_str = String(message);
  message_str += "\n";

  TTLog::appendDateTime(message_str);

  if(print_sd_card)
  {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File DataFile = SD.open(filename, FILE_WRITE);

    // if the file is available, write to it if...
    if (DataFile)
    {
      // an intruder has entered or left...
      if(message_str != NO_DATA && message_str != "")
      {
        DataFile.println(message_str);
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
    Serial.println(message_str);
  }
}

void TTLog::setDateTime(int hour, int minute,
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
  appendDateTime(sNow);
  Serial.println("It is now: ");
  Serial.println(sNow);
}


void TTLog::appendDateTime(String &sDateTime)
{
  appendTime(sDateTime);
  appendDate(sDateTime);
}

void TTLog::appendTime(String &time_str)
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

void TTLog::appendDate(String &date_str)
{
  time_t t = now();
  String month_str = String(monthShortStr(month()));
  date_str += month_str + " ";
  date_str += String(day(t)) + " ";
  date_str += String(year(t)) + "\n";
}

void TTLog::appendDateTimeCSV(String &sDateTime)
{
  appendTimeCSV(sDateTime);
  appendDateCSV(sDateTime);
}

void TTLog::appendDateCSV(String &date_str)
{
  time_t t = now();
  String month_str = String(monthShortStr(month()));
  date_str += month_str + ",";
  date_str += String(day(t)) + ",";
  date_str += String(year(t));
}

void TTLog::appendTimeCSV(String &time_str)
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
