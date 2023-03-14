#include "elevatorfloorsensor.h"

const int ElevatorFloorSensor::INTERVAL_BETWEEN_FLOORS = 4000;

ElevatorFloorSensor::ElevatorFloorSensor() : nextFloor(-1)
{
    connect(&timer, &QTimer::timeout, this, &ElevatorFloorSensor::onTimeout);
}

void ElevatorFloorSensor::detectMovement(int floorNum, Direction direction){
    qDebug() << "Floor sensor detecting movement from floor" << floorNum;
    if (direction == Direction::UP) nextFloor = floorNum + 1;
    else if (direction == Direction::DOWN) nextFloor = floorNum - 1;
    else qDebug() << "Floor sensor detects erroneous elevator movement";
    timer.start(INTERVAL_BETWEEN_FLOORS);
}

void ElevatorFloorSensor::onTimeout(){
    timer.stop();
    emit detectedFloor(nextFloor);
}
