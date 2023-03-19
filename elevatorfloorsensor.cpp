#include "elevatorfloorsensor.h"

const int ElevatorFloorSensor::INTERVAL_BETWEEN_FLOORS = 3000; //Takes 3 seconds between floors

ElevatorFloorSensor::ElevatorFloorSensor() : nextFloor(-1)
{
    connect(&timer, &QTimer::timeout, this, &ElevatorFloorSensor::onTimeout);
}

void ElevatorFloorSensor::detectMovement(int floorNum, Direction direction){
    if (direction == Direction::UP) nextFloor = floorNum + 1; //If going up add 1 to floor number
    else if (direction == Direction::DOWN) nextFloor = floorNum - 1; //If going down subtract 1 from floor number
    else qDebug() << "Floor sensor detects erroneous elevator movement";
    timer.start(INTERVAL_BETWEEN_FLOORS);
}

void ElevatorFloorSensor::onTimeout(){
    timer.stop();
    emit detectedFloor(nextFloor); //Signal that next floor has been reached
}
