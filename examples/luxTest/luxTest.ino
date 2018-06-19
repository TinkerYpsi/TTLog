#include <TT_Log.h>

const int luxDetectorPin = A2;
const int gc1Pin = 5;
const int gc2Pin = 6;
int gc1PinVal = 0;
int gc2PinVal = 0;
int lux = 0;

void setup()
{
  Serial.begin(9600);
  while(!Serial);

  pinMode(gc1Pin, OUTPUT);
  pinMode(gc2Pin, OUTPUT);

  // set lux detector for low gain mode
  digitalWrite(gc1Pin, HIGH);
  digitalWrite(gc2Pin, HIGH);

  Log.begin();
  Log.setDateTime();
  String header = "Humidity,Temperature,Light,Moisture";
  Log.entryHeaderCSV(header, filename);
}

void loop()
{
  lux = analogRead(luxDetectorPin);
  lux *= 100;
  message += String(lux);
}
