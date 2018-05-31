#include <Time.h>
#include <TimeLib.h>

/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */
#define NO_DATA   "NO_DATA"
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
const int motionPin = 7;
int motionState = LOW;    // we start, assuming no motion detected
const int ultrasonicPin = A0;
const int lineTrackPin = 1;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  initialize_SD_card();

  pinMode(motionPin, INPUT);  
  
  setTimeSerial();
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";

  // if motion is detected and previously undetected
  if((digitalRead(motionPin) == HIGH && motionState == LOW)) {
    motionState = HIGH;
    dataString += String("Intruder detected! \n");
    printTimeAndDate(dataString);
  }

  // if motion went away after previously detected
  else if((digitalRead(motionPin) == LOW && motionState == HIGH)) {
    motionState = LOW;
    dataString += String("Intruder gone! \n");
    printTimeAndDate(dataString);
  }

  // if nothing about the system changes
  else {
    dataString = NO_DATA;
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it if...
  if (dataFile) {
    // an intruder has entered or left...
    if(dataString != NO_DATA) {
      dataFile.println(dataString);
      // print to the serial port too:
      Serial.println(dataString);
    }
    // otherwise just close the file
    dataFile.close();
  }
  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void setTimeSerial() {
  char input[5];
  
  Serial.println("Please set the time");
  Serial.println();

  memset(input, '\0', sizeof(input));
  Serial.print("Hour: ");
  while(!Serial.available());
  int i = 0;
  while(Serial.available()) 
  {
    delay(10);
    input[i] = Serial.read();
    i++;
    if(i > 1) 
    {
      break;
    }
  }
  int _hour = atoi(input);
  Serial.println(_hour);

  memset(input, '\0', sizeof(input));
  Serial.print("Minute: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available()) 
  {
    delay(10);
    input[i] = Serial.read();
    i++;
    if(i > 1) 
    {
      break;
    }
  }
  int _minute = atoi(input);
  Serial.println(_minute);

  memset(input, '\0', sizeof(input));
  Serial.print("Month: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available()) 
  {
    delay(10);
    input[i] = Serial.read();
    i++;
    if(i > 1) 
    {
      break;
    }
  }
  int _month = atoi(input);
  Serial.println(_month);

  memset(input, '\0', sizeof(input));
  Serial.print("Day: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available()) 
  {
    delay(10);
    input[i] = Serial.read();
    i++;
    if(i > 1) 
    {
      break;
    }
  }
  int _day = atoi(input);
  Serial.println(_day);

  memset(input, '\0', sizeof(input));
  Serial.print("Year: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available()) 
  {
    delay(10);
    input[i] = Serial.read();
    i++;
    if(i > 3) 
    {
      break;
    }
  }

  int _year = atoi(input);
  Serial.println(_year);
  int _second = 0;
  
  setTime(_hour, _minute, _second, _day, _month, _year);
  
  String _now;
  printTimeAndDate(_now);
  Serial.println("It is now: ");
  Serial.println(_now);
}

void printTimeAndDate(String &dataString) 
{
  String _hour; String _minute;
  time_t t = now();
  if(hour(t) < 10)
  {
    _hour = "0" + String(hour(t));
  }
  else
  {
    _hour = String(hour(t));
  }
  
  if(minute(t) < 10)
  {
    _minute = "0" + String(minute(t));
  }
  else
  {
    _minute = String(minute(t));
  }

  String _month = String(monthShortStr(month()));
  dataString += _hour + ":" + _minute + "\n";
  dataString += _month + " ";
  dataString += String(day(t)) + " ";
  dataString += String(year(t)) + "\n";
}

void initialize_SD_card() 
{
  while (!Serial);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}







