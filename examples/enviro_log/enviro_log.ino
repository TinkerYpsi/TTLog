#include <TTLog.h>
#include <cactus_io_AM2302.h>

const int soilMoisturePin = A1;
const int luxDetectorPin = A2;
const int humiditySensorPin = 3;
int lux = 0;
int moisture = 0;
String message;

// filename destination for logged data
String filename = "data.csv";

AM2302 humidityDetector(humiditySensorPin);

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Log.begin();
  Log.setDateTime();
  String header = "Humidity,Temperature,Light,Moisture";
  Log.entryHeaderCSV(header, filename);
}

void loop()
{
  humidityDetector.readHumidity();
  humidityDetector.readTemperature();

  if(isnan(humidityDetector.humidity) || isnan(humidityDetector.temperature_C))
  {
    Serial.println("Humidity sensor read failure!");
    return;
  }

  message += humidityDetector.humidity;
  message += ",";
  message += humidityDetector.temperature_F;
  message += ",";

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
  // TODO: check if removing commas won't change anything
  if(moisture > 700)
  {
    message += "Damp,";
  }
  else if(moisture > 400)
  {
    message += "Average,";
  }
  else
  {
    message += "Dry,";
  }

  Log.entryCSV_str(message, filename);
}
