#ifndef ECS_H
#define ECS_H

#include<QObject>
#include<QVector>
#include<QTextStream>
#include "floor.h"
#include "elevator.h"
#include "allocationstrategy.h"
#include "safetyservice.h"

// Purpose of class: Used as mediator to control interactions between Elevators and Floors
class ECS : public QObject
{
    Q_OBJECT
public:
    ECS(AllocationStrategy* strategy, QVector<Floor*> *floors, QVector<Elevator*> *elevators);
    ~ECS();

    void handleFire(); //Tells ECS to handle the "Fire" signal
    void handlePowerOut(); //Tells ECS to handle the "Power Out" signal

public slots:
    void allocateElevator(int requestFloor, Direction requestDirection); // Used when Floor makes request for an Elevator going in a particular directon
    void floorServiced(int floorNum, Direction direction); // Used when Floor makes request for an Elevator going in a particular directon
    void handleHelp(Elevator*); // Passes Elevator to SafetyService and sets up timer to make sure responses are heard in time
    void receiveSafetyResponse(); //Informs ECS that safety response was received
    void receiveHelpResponse(); //Informs ECS that help response was received

signals:
    void fire(); //Signal emitted to elevators to inform of fire
    void powerOut(); //Signal emitted to elevators to inform of power outage

private:
    QVector<Floor*> *floors; //The floors in the system
    QVector<Elevator*> *elevators; //The elevators in the system
    AllocationStrategy *strategy; //The allocation strategy that the ECS employs for allocating elevators
    SafetyService *safetyService; //The safety service that the ECS connects elevators calling for help to

    const static int MAX_HELP_RESPONSE_TIME; //The time that can be taken for the elevator and safety service to respond before ECS calls 911
    bool helpResponseReceived; //Keeps track of whether elevator responded after calling for help
    bool safetyResponseReceived; //Keeps track of whether safety service responded after elevator called for help
    Elevator *carGettingHelp; //Holds car that may be getting help

    void onHelpTimerTimeout(); //When timer runs out, it is checked whether elevator and safety service responded
    void call911() const;
};

#endif // ECS_H
