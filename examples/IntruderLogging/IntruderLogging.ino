#include <TT_Log.h>

const int motionPin = 7;
int motionState = LOW;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Log.begin();
  Log.setDateTimeTXT();
  pinMode(motionPin, INPUT);
}

void loop()
{
  // filename destination for logged data
  String fileName = "data.txt";

  // if motion is detected and previously undetected
  if((digitalRead(motionPin) == HIGH && motionState == LOW))
  {
    motionState = HIGH;
    Log.entryTXT("Intruder detected by motion!", fileName);
  }

  // if motion went away after previously detected
  else if((digitalReadTXT(motionPin) == LOW && motionState == HIGH))
  {
    motionState = LOW;
    Log.entryTXT("No more motion!", fileName);
  }

  else;   // nothing about the system has changed
}
