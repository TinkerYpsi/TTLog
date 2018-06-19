#include <TT_Log.h>

const int luxDetectorPin = A2;
const int gc1Pin = 5;
const int gc2Pin = 6;
int gc1PinVal = 0;
int gc2PinVal = 0;
int lux = 0;

int detectLux(int gc1Pin, int gc2Pin, int luxDetectorPin, bool debug = false);

void setup()
{
  Serial.begin(9600);
  while(!Serial);
}

void loop()
{
  int lux = detectLux(gc1Pin, gc2Pin, luxDetectorPin, true);
  Serial.println(lux);
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
