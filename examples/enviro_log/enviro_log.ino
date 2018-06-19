#include <TT_Log.h>
#include <cactus_io_AM2302.h>

const int soilMoisturePin = A1;
const int luxDetectorPin = A2;
const int humiditySensorPin = 3;
const int gc1Pin = 5;
const int gc2Pin = 6;
int gc1PinVal = 0;
int gc2PinVal = 0;
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

  pinMode(gc1Pin, OUTPUT);
  pinMode(gc2Pin, OUTPUT);
  digitalWrite(gc1Pin, HIGH);
  digitalWrite(gc2Pin, HIGH);

  Log.begin();
  Log.setDateTime();
  // TODO: only add once b/c we're appending to existing file
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
  gc1PinVal = digitalRead(gc1Pin);
  gc2PinVal = digitalRead(gc2Pin);
  determineBrightness(lux, gc1PinVal, gc2PinVal, message);

  moisture = analogRead(soilMoisturePin);
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

  Log.entryCSV(message, filename);
  message = "";
  delay(5000);
}

void determineBrightness(int lux, int gc1PinVal, int gc2PinVal, String &message)
{
  // // L-gain: ~100,000 lux range
  // if(gc1PinVal && gc2PinVal)
  // {
  //
  // }
  // // M-gain: ~10,000 lux range
  // else if(gc2PinVal)
  // {
  //
  // }
  // // H-gain: ~1,000 lux range
  // else
  // {
  //
  // }
  //
  
  lux *= 100;
  message += String(lux);
  message += ",";

  // // 100,000 lx = 1023 analog output -- 1000 uA, 5000 ohm, 5V
  // if(lux == 1023)
  // {
  //   message += "100000,";
  // }
  //
  // // 10,000 lx = 102 analog output -- 100 uA, 5000 ohm, 0.5V (10% of 5V)
  // if(lux == 102.3)
  // {
  //   message += "10000,";
  // }
  //
  // // 1000 lx = 10 analog output -- 10 uA, 5000 ohm, 0.05V (1% of 5V)
  // if(lux == 10.23)
  // {
  //   message += "1000,";
  // }
  //
  // // 100 lx -- 1 uA, 5000 ohm, 0.005V (0.1% of 5V)
  // if(lux == 1.023)
  // {
  //   message += "100,";
  // }
}
