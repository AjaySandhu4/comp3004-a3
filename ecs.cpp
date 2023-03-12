#include "ecs.h"

ECS::ECS(QVector<Floor*> *floors, QVector<Elevator*> *elevators)
    : floors(floors)
    , elevators(elevators)
{ }

void ECS::processFloorRequest(int floorNum, char direction)
{
    QTextStream(stdout) << "ECS processing floor request for floor " << floorNum << " in direction " << endl;
}
