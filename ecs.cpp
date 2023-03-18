#include "ecs.h"

ECS::ECS(AllocationStrategy* strategy, QVector<Floor*> *floors, QVector<Elevator*> *elevators)
    : floors(floors)
    , elevators(elevators)
    , strategy(strategy)
{
    for(int i=0; i<floors->length(); ++i){
        connect(floors->value(i), &Floor::requestForElevator, this, &ECS::allocateElevator);
    }
    for(int i=0; i<elevators->length(); ++i){
        connect(elevators->value(i), &Elevator::floorServiced, this, &ECS::floorServiced);
    }
}

void ECS::allocateElevator(int requestFloor, Direction requestDirection)
{
    strategy->allocateElevator(requestFloor, requestDirection, elevators, floors->length());
}

void ECS::floorServiced(int floorNum, Direction direction)
{
    floors->value(floorNum)->serviced(direction);
}
