#ifndef TIMEDEPENDENTSTRATEGY_H
#define TIMEDEPENDENTSTRATEGY_H

#include "allocationstrategy.h"

class TimeDependentStrategy : public AllocationStrategy
{
    Q_OBJECT
public slots:
    void allocateElevator(int requestFloor, Direction requestDirection, QVector<Elevator*> *cars, int totalFloors);
    void rerouteElevatorRequests(Elevator* elevator, QVector<Elevator*> *cars, QVector<Floor*> *floors); //TODO: probably bad
};

#endif // TIMEDEPENDENTSTRATEGY_H
