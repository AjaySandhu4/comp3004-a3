#ifndef TIMEDEPENDENTSTRATEGY_H
#define TIMEDEPENDENTSTRATEGY_H

#include <QTime>
#include "elevatorcenteredstrategy.h"

// Purpose of class: Concrete implementation of the allocation strategy of only having Floor requests routed to the ECS (will be used as default)
class TimeDependentStrategy : public ElevatorCenteredStrategy
{
    Q_OBJECT
public slots:
    void allocateElevator(int requestFloor, Direction requestDirection, QVector<Elevator*> *cars, int totalFloors);
};

#endif // TIMEDEPENDENTSTRATEGY_H
