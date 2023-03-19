#ifndef ELEVATORCENTEREDSTRATEGY_H
#define ELEVATORCENTEREDSTRATEGY_H

#include "allocationstrategy.h"

// Purpose of class: Concrete implementation of the allocation strategy of only having Floor requests routed to the ECS (will be used as default)
class ElevatorCenteredStrategy : public AllocationStrategy
{
    Q_OBJECT

public slots:
    void allocateElevator(int requestFloor, Direction requestDirection, QVector<Elevator*> *cars, int totalFloors);
};

#endif // ELEVATORCENTEREDSTRATEGY_H
