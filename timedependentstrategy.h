#ifndef TIMEDEPENDENTSTRATEGY_H
#define TIMEDEPENDENTSTRATEGY_H

#include "allocationstrategy.h"

class TimeDependentStrategy : public AllocationStrategy
{
    Q_OBJECT
public slots:
    void allocateElevator(int floorNum, Direction direction, QVector<Elevator*> *cars);
    void rerouteElevatorRequests(Elevator* elevator, QVector<Elevator*> *cars, QVector<Floor*> *floors); //TODO: probably bad
};

#endif // TIMEDEPENDENTSTRATEGY_H
