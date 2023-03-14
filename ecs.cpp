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

void ECS::allocateElevator(int floorNum, Direction direction)
{
    QTextStream(stdout) << "ECS allocating elevator for floor " << floorNum << " in direction " << direction << endl;
    strategy->allocateElevator(floorNum, direction, elevators);
}

void ECS::floorServiced(int floorNum, Direction direction)
{
    Floor* servicedFloor = floors->value(floorNum);
    if(servicedFloor->isTop()) servicedFloor->serviced(Direction::DOWN); //If elevator was servicing top floor, the floor can only be serviced to go DOWN
    else if(servicedFloor->getLevel() == Floor::GROUND_LEVEL) servicedFloor->serviced(Direction::UP); //If elevator was servicing ground floor, the floor can only be serviced to go UP
    else servicedFloor->serviced(direction);
//    floors->value(floorNum)->serviced(direction);
}
