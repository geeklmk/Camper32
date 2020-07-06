// Camper32 Ptoject by Luca Marchetti
// Caravan Automation, replacement for the old electronics of a Laika Ecovip 4
//
// REQUIRES the following Arduino libraries:
// - Arduino Timer: https://github.com/contrem/arduino-timer
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <arduino-timer.h>

#define DHTPIN 4
#define DHTTYPE DHT22

// DHT Sensor init
DHT dht(DHTPIN, DHTTYPE);
// Setting up timer library
auto timer = timer_create_default();

// Global Data
float humidity = 0;
float temperature = 0;
float heatIndex = 0;

void setup()
{

  // Opening Console Debug Serial
  Serial.begin(9600);
  Serial.println(F("Camper32 Project By Luca Marchetti"));
  Serial.println(F("Booting sequence"));

  Serial.println(F("DHT22 temperature sensor init"));
  dht.begin();
  _readDHT22();
  Serial.println(F("Setup timed temperature read. 5sec interval"));
  timer.every(20000, readDHT22);
  
}

bool readDHT22(void *) 
{
  return _readDHT22();
}
bool _readDHT22() 
{
  // read with defaults. (Celsius)
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) ) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return true;
  }
  // Heat index
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C "));
  Serial.print(F(" Heat index: "));
  Serial.print(heatIndex);
  Serial.println(F("°C "));

  return true; // keep timer active? true
}

void loop() 
{
  // check the timed funcions
  timer.tick(); 
}
