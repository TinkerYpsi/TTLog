#include <TTLog.h>

const int soilMoisturePin = A1;
const int luxDetectorPin = A2;
int lux = 0;
int moisture = 0;

const int
int motionState = LOW;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Log.begin();
  Log.setDateTimeCSV();
  pinMode(motionPin, INPUT);
}

void loop()
{
  // filename destination for logged data
  String fileName = "data.csv";
  lux = analogRead(luxDetectorPin);
  moisture = analogRead(soilMoisturePin);
  String message;

  if(lux > 700)
  {
    message += "Bright light,";
  }
  else if(lux > 400)
  {
    message += "Medium light,";
  }
  else
  {
    message += "Low light,";
  }

  if(moisture > 700)
  {
    message += "Damp,"
  }
  else if(mositure > 400)
  {
    message += "Average,"
  }
  else
  {
    message += "Dry,"
  }

  Log.entryCSV(message, fileName);
}
