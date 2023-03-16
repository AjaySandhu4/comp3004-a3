#include "doorlightsensor.h"

DoorLightSensor::DoorLightSensor() : active(false)
{ }

void DoorLightSensor::setActive(bool active){ this->active = active; }

void DoorLightSensor::detects(){
    if(active) emit triggered();
}


