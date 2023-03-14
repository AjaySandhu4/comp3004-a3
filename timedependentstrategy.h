#ifndef TIMEDEPENDENTSTRATEGY_H
#define TIMEDEPENDENTSTRATEGY_H

#include "allocationstrategy.h"

class TimeDependentStrategy : public AllocationStrategy
{
    Q_OBJECT
public slots:
    void processFloorRequest(int floorNum, Direction direction, Elevator** cars);
    void rerouteElevatorRequests(Elevator* elevator, Elevator** cars, Floor** floors);
};

#endif // TIMEDEPENDENTSTRATEGY_H
