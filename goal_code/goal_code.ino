#include <TTLog.h>

const int g_iMotionPin = 7;
int g_iMotionState = LOW;

void setup()
{
  Serial.begin(9600);
  TTLog::log()->setDateTime();
  pinMode(g_iMotionPin, INPUT);
}

void loop()
{
  // filename destination for logged data
  String g_sFileName = "datalog.txt";

  // if motion is detected and previously undetected
  if((digitalRead(g_iMotionPin) == HIGH && g_iMotionState == LOW))
  {
    g_iMotionState = HIGH;
    TTLog::log()->entry("Intruder detected by motion!", g_sFileName);
  }

  // if motion went away after previously detected
  else if((digitalRead(g_iMotionPin) == LOW && g_iMotionState == HIGH))
  {
    g_iMotionState = LOW;
    TTLog::log()->entry("No more motion!", g_sFileName);
  }

  else;   // nothing about the system has changed
}
