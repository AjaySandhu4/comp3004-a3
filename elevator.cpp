#include "elevator.h"

Elevator::Elevator(int carNum, int numFloors)
    : carNum(carNum)
    , currFloor(0)
    , floorRequests(new bool[numFloors]{0})
    , doorsOpen(true)
    , state(ElevatorState::IDLE)
{ }

Elevator::~Elevator()
{
    delete floorRequests;
}

bool Elevator::isRequestedFloor(int floorNum) const { return floorRequests[floorNum]; }

void Elevator::destFloorRequest(int floorNum)
{
    floorRequests[floorNum] = true;
    std::cout << "Car number " << carNum << " has received request for floor " << floorNum << std::endl;
}

void Elevator::newFloor(int floorNum)
{
    currFloor = floorNum;
    std::cout << "Car number " << carNum << " has reached floor " << currFloor << std::endl;
}
