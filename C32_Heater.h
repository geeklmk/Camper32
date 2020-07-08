/*
  C32_Heater.h - Library for handling Laika Ecovip water heater
  Created by Luca Marchetti for the Camper32 Personal Project
  Released into the public domain.
*/

#ifndef C32Heater_h
#define C32Heater_h

#include "Arduino.h"

class C32Heater
{
  public:
    typedef void (*C32heaterEvent) ();
    enum HeaterTemperature { C50, C70, NONE };
    enum HeaterStatus { Off, Starting, On }; 

    C32heaterEvent onHeaterStarting;
    C32heaterEvent onHeaterStarted;
    C32heaterEvent onHeaterStopped;
    C32heaterEvent onHeaterError;
    
    C32Heater(int pin50, int pin70, int pinLedStarting, int pinLedOn);
    
    void set50();
    void set70();
    void stopHeater();
    void updateHeater();
    HeaterTemperature getRequestedTemperature();
    HeaterTemperature getActualTemperature();
    HeaterStatus getStatus();
  private:
    int _pin50;
    int _pin70;
    int _pinLedStarting;
    int _pinLedOn;
    int _lastLedOn;
    int _lastLedStarting;
    HeaterTemperature _requestedTemperature;
    HeaterTemperature _actualTemperature;
    HeaterStatus _status;
};
#endif
