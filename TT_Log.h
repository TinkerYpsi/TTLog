#ifndef TT_Log_h
#define TT_Log_h

#define DEFAULT_CS_PIN  10
#define NO_DATA   "NO_DATA"

#include <Arduino.h>

#include <SPI.h>
#include <SD.h>
#include <Time.h>
#include <TimeLib.h>

/*
 *  NOTE: This library initializes communication with the SD card,
 *  so if you wish to call other functions from the SD library, there is no
 *  need to call SD.begin() (and in fact, it will fail if you do).
 */

/*
* SD card attached to SPI bus as follows:
** CS - pin 10 by default, but can be changed
** MOSI - pin 11
** MISO - pin 12
** CLK - pin 13
*/

class TT_Log
{
  public:

    TT_Log();

    /* by default, user sets time using Serial.read()
       but user can also enter time manually as params */
    void setDateTime(int hour = -1, int minute = -1, int day = -1,
                            int month = -1, int year = -1);

    /*
     * prints message to filename on SD card and serial by default
     * formatted for .txt files
     */
    void entryTXT(const char *message, String &filename,
                  bool print_serial = true, bool print_sd_card = true);

    void entryTXT(String &message, String &filename,
                  bool print_serial = true, bool print_sd_card = true);

    /*
     * prints message to filename on SD card and serial by default
     * formatted for .csv files
     */
    void entryCSV(const char *message, String &filename,
                  bool print_serial = true, bool print_sd_card = true);

    void entryCSV(String &message, String &filename,
                  bool print_serial = true, bool print_sd_card = true);

    void entryHeaderCSV(String &header, String &filename,
                        bool print_serial = true, bool print_sd_card = true);

    /* concatenates string with current time and/or date for .txt files */
    void appendDateTimeTXT(String &date_time);
    void appendTimeTXT(String &time);
    void appendDateTXT(String &date);
    /* concatenates string using commas for .csv files */
    void appendDateTimeCSV(String &date_time);
    void appendTimeCSV(String &time);
    void appendDateCSV(String &date);
    void begin(uint8_t sd_cs_pin = DEFAULT_CS_PIN);

  private:
    void print(String &message, String &filename,
               bool print_serial, bool print_sd_card);
};

extern TT_Log Log;

#endif
