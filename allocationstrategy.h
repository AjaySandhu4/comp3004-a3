#ifndef ALLOCATIONSTRATEGY_H
#define ALLOCATIONSTRATEGY_H

#include <QObject>
#include <QVector>
#include <elevator.h>
#include <floor.h>
#include <direction.h>
#include <elevatorstate.h>

class AllocationStrategy : public QObject
{
    Q_OBJECT
public:
    virtual ~AllocationStrategy();

public slots:
    virtual void allocateElevator(int floorNum, Direction direction, QVector<Elevator*> *cars) = 0;
    virtual void rerouteElevatorRequests(Elevator* elevator, QVector<Elevator*> *cars, QVector<Floor*> *floors) = 0;

};

#endif // ALLOCATIONSTRATEGY_H
