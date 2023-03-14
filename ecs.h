#ifndef ECS_H
#define ECS_H

#include<QObject>
#include<QVector>
#include<QTextStream>
#include "floor.h"
#include "elevator.h"
#include "allocationstrategy.h"

class ECS : public QObject
{
    Q_OBJECT
public:
    ECS(AllocationStrategy* strategy, QVector<Floor*> *floors, QVector<Elevator*> *elevators);

public slots:
    void processFloorRequest(int floorNum, char direction);

private:
    QVector<Floor*> *floors;
    QVector<Elevator*> *elevators;
    AllocationStrategy *strategy;


};

#endif // ECS_H
