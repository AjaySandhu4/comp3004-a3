#ifndef ALLOCATIONSTRATEGY_H
#define ALLOCATIONSTRATEGY_H

#include <QObject>
#include <QVector>
#include <elevator.h>
#include <floor.h>
#include <direction.h>
#include <elevatorstate.h>

// Purpose of class: Interface for letting the allocation strategy become an attribute to make use of strategy design pattern
class AllocationStrategy : public QObject
{
    Q_OBJECT
public:
    virtual ~AllocationStrategy();

public slots:
    //Method used to allocate request from a floor to an elevator
    virtual void allocateElevator(int requestFloor, Direction requestDirection, QVector<Elevator*> *cars, int totalFloors) = 0;
};

#endif // ALLOCATIONSTRATEGY_H
