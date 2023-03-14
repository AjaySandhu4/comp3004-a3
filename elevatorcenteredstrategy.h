#ifndef ELEVATORCENTEREDSTRATEGY_H
#define ELEVATORCENTEREDSTRATEGY_H

#include "allocationstrategy.h"

class ElevatorCenteredStrategy : public AllocationStrategy
{
    Q_OBJECT

public slots:
    void allocateElevator(int floorNum, Direction direction, QVector<Elevator*> *cars);
    void rerouteElevatorRequests(Elevator* elevator, QVector<Elevator*> *cars, QVector<Floor*> *floors);
};

#endif // ELEVATORCENTEREDSTRATEGY_H
