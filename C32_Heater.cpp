#include "Arduino.h"
#include "C32_Heater.h"

C32Heater::C32Heater(int pin50, int pin70, int pinLedStarting, int pinLedOn)
{
  _pin50 = pin50;
  _pin70 = pin70;
  _pinLedStarting = pinLedStarting;
  _pinLedOn = pinLedOn;
  _lastLedOn = 0;
  _lastLedStarting = 0;
  _status = HeaterStatus::Off;
  _requestedTemperature = HeaterTemperature::NONE;
  _actualTemperature = HeaterTemperature::NONE;

  pinMode( _pin50, OUTPUT );
  pinMode( _pin70, OUTPUT );
  pinMode( _pinLedStarting, INPUT);
  pinMode( _pinLedOn, INPUT );
}

void C32Heater::set50()
{
  digitalWrite( _pin70, LOW );
  digitalWrite( _pin50, HIGH );
  _requestedTemperature = HeaterTemperature::C50;
  if( _status = HeaterStatus::On )
    _actualTemperature = _requestedTemperature;
}

void C32Heater::set70()
{
  digitalWrite( _pin50, LOW );
  digitalWrite( _pin70, HIGH );
  _requestedTemperature = HeaterTemperature::C50;
  if( _status = HeaterStatus::On )
    _actualTemperature = _requestedTemperature;
}

void C32Heater::stopHeater()
{
  digitalWrite( _pin50, LOW );
  digitalWrite( _pin70, LOW );  
  _requestedTemperature = HeaterTemperature::NONE;
  if( _status = HeaterStatus::Off )
    _actualTemperature = _requestedTemperature;
}

C32Heater::HeaterTemperature C32Heater::getRequestedTemperature()
{
  return _requestedTemperature;
}


C32Heater::HeaterTemperature C32Heater::getActualTemperature()
{
  return _actualTemperature;
}

C32Heater::HeaterStatus C32Heater::getStatus()
{
  return _status;
}

void C32Heater::updateHeater()
{
  int _on;
  int _starting;
  _on = digitalRead( _pinLedOn );
  _starting = digitalRead( _pinLedStarting );

  if( onHeaterStarting && _lastLedStarting == LOW && _starting == HIGH )
  {
    _status = HeaterStatus::Starting;
    onHeaterStarting();
  }
  
  if( onHeaterStarted && _lastLedOn == LOW && _on == HIGH )
  {
    _status = HeaterStatus::On;
    _actualTemperature = _requestedTemperature;
    onHeaterStarted();
  }
  
   if( onHeaterStopped && _lastLedOn == HIGH && _on == LOW )
   {
    _status = HeaterStatus::Off;
    _actualTemperature = HeaterTemperature::NONE;
    onHeaterStopped();
   }
   
  _lastLedOn = _on;
  _lastLedStarting = _starting;
}
