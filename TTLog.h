#ifndef TTLog_h
#define TTLog_h

#define DEFAULT_CS_PIN  10
#define NO_DATA   "NO_DATA"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Time.h>
#include <TimeLib.h>

/*
* SD card attached to SPI bus as follows:
** MOSI - pin 11
** MISO - pin 12
** CLK - pin 13
*/

class TTLog
{
  public:
    static TTLog& create()
    {
      static TTLog s_object;
      return s_object;
    }

    /* by default, user sets time using Serial.read()
       but user can also enter time manually as params */
    void setDateTime(int &iHour = -1, int &iMinute = -1, int &iDay = -1,
                     int &iMonth = -1, int &iYear = -1);

    /* prints message to sFilename on SD card and serial by default */
    void entry(String &sMessage, String &sFilename,
               bool &bPrintSerial = true, bool &bPrintSDCard = true);

    /* concatenates string with current time and/or date */
    void printDateTime(String &sDateTime);
    void printTime(string &sTime);
    void printDate(string &sDate);

  private:
    TTLog() {}
    int m_cs_pin;

  public:
    TTLog(TTlog const&)           = delete;   // prevents copying a singleton
    void operator=(TTLog const&)  = delete;   // prevents copying a singleton
};

#endif
