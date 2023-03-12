#ifndef ECS_H
#define ECS_H

#include<QObject>
#include<QVector>
#include<QTextStream>
#include "floor.h"
#include "elevator.h"

class ECS : public QObject
{
    Q_OBJECT
public:
    ECS(QVector<Floor*> *floors, QVector<Elevator*> *elevators);

public slots:
    void processFloorRequest(int floorNum, char direction);

private:
    QVector<Floor*> *floors;
    QVector<Elevator*> *elevators;


};

#endif // ECS_H
