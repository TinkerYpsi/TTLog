#include <TTLog.h>

const int soilMoisturePin = A1;
const int luxDetectorPin = A2;
const int humiditySensorPin = 3;
int lux = 0;
int moisture = 0;
String message;

AM2302 humidityDetector(humiditySensorPin);

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Log.begin();
  Log.setDateTimeCSV();
}

void loop()
{
  // filename destination for logged data
  String fileName = "data.csv";

  humidityDetector.readHumidity();
  humidityDetector.readTemperature();

  if(isnan(humidityDetector.humidity) || isnan(humidityDetector.temperature_C))
  {
    Serial.println("Humidity sensor read failure!");
    return;
  }

  message += humidityDetector.humidity + ",";
  message += humidityDetector.temperature_F + ",";

  lux = analogRead(luxDetectorPin);
  moisture = analogRead(soilMoisturePin);

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
  else if(moisture > 400)
  {
    message += "Average,"
  }
  else
  {
    message += "Dry,"
  }

  Log.entryCSV(message, fileName);
}
