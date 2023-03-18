#include "weightsensor.h"

const int WeightSensor::MAX_WEIGHT_TOLERANCE = 400; //Elevator can only tolerate 400kg before being overloaded

WeightSensor::WeightSensor() : weight(0)
{ }

int WeightSensor::overloadedBy() const { return weight - MAX_WEIGHT_TOLERANCE; }
bool WeightSensor::isLoaded() const { return weight != 0; }

void WeightSensor::addWeight(int weightAdded){
    weight += weightAdded;
    if(overloadedBy() > 0)
        emit overload();
}

void WeightSensor::reduceWeight(int weightReduced){
    if(weight >= weight-weightReduced)
        weight -= weightReduced;
}
