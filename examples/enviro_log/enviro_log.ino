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

int detectLux(int gc1Pin, int gc2Pin, int luxDetectorPin, bool debug = false);

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

  int lux = detectLux(gc1Pin, gc2Pin, luxDetectorPin);
  message += String(lux);
  message += ",";

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

int detectLux(int gc1Pin, int gc2Pin, int luxDetectorPin, bool debug = false)
{
  pinMode(gc1Pin, OUTPUT);
  pinMode(gc2Pin, OUTPUT);

  // set lux detector for low gain mode
  digitalWrite(gc1Pin, HIGH);
  digitalWrite(gc2Pin, HIGH);
  int lux = analogRead(luxDetectorPin);
  lux *= 100;

  // if less than 1,000 lx, switch to high gain
  if(lux < 1000)
  {
    // set lux detector for high gain mode
    digitalWrite(gc1Pin, HIGH);
    digitalWrite(gc2Pin, LOW);
    lux = analogRead(luxDetectorPin);
    if(debug)
    {
      Serial.println("HIGH GAIN MODE");
      delay(1000);
    }
    return lux;
  }

  // if less than 10,000 lx, switch to medium gain
  else if(lux < 10000)
  {
    // set lux detector for medium gain mode
    digitalWrite(gc1Pin, LOW);
    digitalWrite(gc2Pin, HIGH);
    lux = analogRead(luxDetectorPin);
    lux *= 10;
    if(debug)
    {
      Serial.println("MEDIUM GAIN MODE");
      delay(1000);
    }
    return lux;
  }

  // if greater than 10,000 lx, stay in high gain
  else
  {
    if(debug)
    {
      Serial.println("LOW GAIN MODE");
      delay(1000);
    }
    return lux;
  }
}

// HIGH GAIN MODE
// 100 lx = 102 analog output -- 100 uA, 5000 ohm, 0.5V (10% of 5V)
// 10 lx = 10 analog output -- 10 uA, 5000 ohm, 0.05V (1% of 5V)
// 1 lx = 1 analog output -- 1 uA, 5000 ohm, 0.005V (0.1% of 5V)

// MEDIUM GAIN MODE
// 100 lx = 10 analog output -- 10 uA, 5000 ohm, 0.05V (1% of 5V)
// 1,000 lx = 102 analog output -- 100 uA, 5000 ohm, 0.5V (10% of 5V)
// 10,000 lx = 1023 analog output -- 1000 uA, 5000 ohm, 5V

// LOW GAIN MODE
// 100 lx = 1 analog output -- 1 uA, 5000 ohm, 0.005V (0.1% of 5V)
// 1,000 lx = 10 analog output -- 10 uA, 5000 ohm, 0.05V (1% of 5V)
// 10,000 lx = 102 analog output -- 100 uA, 5000 ohm, 0.5V (10% of 5V)
// 100,000 lx = 1023 analog output -- 1000 uA, 5000 ohm, 5V
