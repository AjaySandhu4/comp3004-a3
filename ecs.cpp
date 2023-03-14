#include "ecs.h"

ECS::ECS(AllocationStrategy* strategy, QVector<Floor*> *floors, QVector<Elevator*> *elevators)
    : strategy(strategy)
    , floors(floors)
    , elevators(elevators)
{ }

void ECS::processFloorRequest(int floorNum, char direction)
{
    QTextStream(stdout) << "ECS processing floor request for floor " << floorNum << " in direction " << endl;
}
