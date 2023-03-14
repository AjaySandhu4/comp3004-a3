#ifndef ELEVATORCENTEREDSTRATEGY_H
#define ELEVATORCENTEREDSTRATEGY_H

#include "allocationstrategy.h"

class ElevatorCenteredStrategy : public AllocationStrategy
{
    Q_OBJECT

public slots:
    void processFloorRequest(int floorNum, Direction direction, Elevator** cars);
    void rerouteElevatorRequests(Elevator* elevator, Elevator** cars, Floor** floors);
};

#endif // ELEVATORCENTEREDSTRATEGY_H
