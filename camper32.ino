// Camper32 Ptoject by Luca Marchetti
// Caravan Automation, replacement for the old electronics of a Laika Ecovip 4
//
// REQUIRES the following Arduino libraries:
// - Arduino Timer: https://github.com/contrem/arduino-timer
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <arduino-timer.h>
#include "C32_Heater.h"

// Temperature and Humidity Sensor
#define DHT_PIN 4
#define DHTTYPE DHT22

// Water Heater pins
#define WH_C50_PIN        5
#define WH_C70_PIN        6
#define WH_STARTING_PIN   7
#define WH_ON_PIN         8

// Water Pump pins
#define WATER_PUMP_PIN    9
#define LIGHTS_PIN        10

// DHT Sensor init
DHT dht(DHT_PIN, DHTTYPE);

// Heater 
C32Heater heater( WH_C50_PIN, WH_C70_PIN, WH_STARTING_PIN, WH_ON_PIN );

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
  
  Serial.println(F("Water Heater controller init"));
  heater.stopHeater();
  heater.onHeaterStarting = onHeaterStarting;
  heater.onHeaterStarted  = onHeaterStarted;
  heater.onHeaterStopped  = onHeaterStopped;
  
  Serial.println(F("Setup timed temperature read. 5sec interval"));
  timer.every(20000, readDHT22);

  Serial.println(F("Starting Operations"));
}

// Water Heater Callbacks
void onHeaterStarting()
{
  Serial.println(F("Heater starting"));
}

void onHeaterStarted()
{
  Serial.println(F("Heater started"));
}

void onHeaterStopped()
{
  Serial.println(F("Heater stopped"));
}

// helper function for scheduling
bool readDHT22(void *) 
{
  return _readDHT22();
}
// real funcion for reading sensor
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
    
  debugLog_DHT22();
  return true; // keep timer active? true
}

void loop() 
{
  // check the timed funcions
  timer.tick(); 
  heater.updateHeater();
}

// helper debug log functions

void debugLog_DHT22()
{
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C "));
  Serial.print(F(" Heat index: "));
  Serial.print(heatIndex);
  Serial.println(F("°C "));
}
