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

class TTLog
{
  public:

    TTLog();

    /* by default, user sets time using Serial.read()
       but user can also enter time manually as params */
    void setDateTime(int hour = -1, int minute = -1, int day = -1,
                            int month = -1, int year = -1);

    /* prints message to sFilename on SD card and serial by default */
    void entry(const char *message, String &filename,
                      bool print_serial = true, bool print_sd_card = true);

    /* concatenates string with current time and/or date */
    void appendDateTime(String &date_time);
    void appendTime(String &time);
    void appendDate(String &date);
    void begin(uint8_t sd_cs_pin = DEFAULT_CS_PIN);

  private:
};

extern TTLog Log;

#endif
