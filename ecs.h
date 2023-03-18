#ifndef ECS_H
#define ECS_H

#include<QObject>
#include<QVector>
#include<QTextStream>
#include "floor.h"
#include "elevator.h"
#include "allocationstrategy.h"
#include "safetyservice.h"

class ECS : public QObject
{
    Q_OBJECT
public:
    ECS(AllocationStrategy* strategy, QVector<Floor*> *floors, QVector<Elevator*> *elevators);
    ~ECS();

    void handleFire();
    void handlePowerOut();

public slots:
    void allocateElevator(int requestFloor, Direction requestDirection);
    void floorServiced(int floorNum, Direction direction);
    void handleHelp(Elevator*);
    void receiveSafetyResponse();
    void receiveHelpResponse();

signals:
    void fire();
    void powerOut();

private:
    QVector<Floor*> *floors;
    QVector<Elevator*> *elevators;
    AllocationStrategy *strategy;
    SafetyService *safetyService;

    const static int MAX_HELP_RESPONSE_TIME;
    bool helpResponseReceived;
    bool safetyResponseReceived;
    Elevator *carGettingHelp;

    void onHelpTimerTimeout();
    void call911() const;
};

#endif // ECS_H
