#include <TTLog.h>

const int motionPin = 7;
int motionState = LOW;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Log.begin();
  Log.setDateTime();
  pinMode(g_iMotionPin, INPUT);
}

void loop()
{
  // filename destination for logged data
  String fileName = "data.txt";

  // if motion is detected and previously undetected
  if((digitalRead(motionPin) == HIGH && motionState == LOW))
  {
    motionState = HIGH;
    Log.entry("Intruder detected by motion!", fileName);
  }

  // if motion went away after previously detected
  else if((digitalRead(motionPin) == LOW && motionState == HIGH))
  {
    motionState = LOW;
    Log.entry("No more motion!", fileName);
  }

  else;   // nothing about the system has changed
}
