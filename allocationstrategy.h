#ifndef ALLOCATIONSTRATEGY_H
#define ALLOCATIONSTRATEGY_H

#include <QObject>
#include <elevator.h>
#include <floor.h>
#include <direction.h>

class AllocationStrategy : public QObject
{
    Q_OBJECT
public:
    virtual ~AllocationStrategy();

public slots:
    virtual void processFloorRequest(int floorNum, Direction direction, Elevator** cars) = 0;
    virtual void rerouteElevatorRequests(Elevator* elevator, Elevator** cars, Floor** floors) = 0;

};

#endif // ALLOCATIONSTRATEGY_H
