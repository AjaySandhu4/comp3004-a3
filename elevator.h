#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <iostream>
#include "elevatorfloorsensor.h"
#include "elevatorstate.h"
#include "direction.h"
#include "weightsensor.h"
#include "doorlightsensor.h"

// Purpose of class: Each instantiation of the Elevator class represents one of the M cars of the elevator system
class Elevator : public QObject
{
    Q_OBJECT
public:
    Elevator(int carNum, int numFloors);
    ~Elevator();

    //getters
    bool isRequestedFloor(int floorNum, Direction requestDirection = Direction::UNKNOWN) const;
    int getCurrFloor() const;
    int getCarNum() const;
    ElevatorState getState() const;
    Direction getDirection() const;
    bool isEmpty() const;
    WeightSensor* getWeightSensor() const;
    DoorLightSensor* getDoorLightSensor() const;
    bool areDoorsOpen() const;
    bool isOutOfService() const;

    void onOpenButtonPress(); //Keeps doors open longer than their defined period
    void onCloseButtonRelease(); //Closes doors earlier than their defined period
    void handleHelp(); //Informs elevator that help button was pressed
    void receiveMicInput(); //Mic input received means that caller for help has given a response

public slots:
    void destFloorRequest(int floorNum, Direction requestDirection = Direction::UNKNOWN); //Adds request for floor in a particular direction if specified
    void newFloor(int floorNum); //Informs elevator that new floor has been reached
    void handleOverload(); //Informs elevator that an overload has occured
    void handleDoorObstacle(); //Informs elevator that a door obstacle was encountered
    void handleFire(); //Informs elevator of a fire
    void handlePowerOut(); //Informs elevator of a power outage

private slots:
    void doorsHaveShut(); //Informs elevator that the doors have finished closing and are now shut
    void doorTimerFinished(); //Informs elevator to begin door closing sequence (if possible)
    void move(); //Informs elevator to begin/continue moving

signals:
    void moving(int startingFloor, Direction direction); //Elevator signals that it is moving from a starting floor in a particular direction
    void floorServiced(int floorNum, Direction direction); //Elevator signals that is has serviced a floor in a particular direction
    void reachedFloor(int floorNum);
    void voice(const QString&); //Elevator voices message to "Audio Ouput"
    void textMessage(const QString&); //Elevator sends text message to "Text Display"
    void callingForHelp(Elevator*); //Elevator signals that it is in need of help from safety service
    void helpFollowUp(); //Elevator signals that that the help call was followed up by the passenger in the elevator

private:
    const int carNum; //The number of the elevator (starts from zero to M-1)
    const int numFloors; //The number of floors in the building the elevator is in
    int currFloor; //The floor is elevator is currently at
    char *floorRequests; //The array holding the requests for floors the elevator has where the direction is represented by REQUESTED_UP and REQUESTED_DOWN
    bool doorsOpen;
    ElevatorState state;
    Direction direction; //Direction elevator is/will travel
    ElevatorFloorSensor floorSensor;
    DoorLightSensor *doorLightSensor;
    WeightSensor *weightSensor;
    QTimer doorTimer; // Timer used to keep doors open to allow passengers to enter and exit
    QTimer closeDoorsTimer; // Timer used to keep track of when doors are in process of closing
    int consecutiveDoorInterruptions; //Keeps track of how many times consecutively a door obstacle is met
    bool outOfService;

    const static int DOOR_TIMER_INTERVAL; //Time it takes for elevator to try and close its doors
    const static int TIME_DOORS_TAKE_TO_SHUT; //Time it takes 2.5 seconds from elevator telling doors to close to them actually being shut
    const static char REQUESTED_UP; //Bits that represents that a floor request exists in the UP direction
    const static char REQUESTED_DOWN; //Bits that represents that a floor request exists in the DOWN direction
    const static int SAFE_FLOOR; //Designated safe floor

    void stop();
    void closeDoors();
    void openDoors();
    void decideDirectionToGo();
    bool areFloorRequestsEmpty() const;
    void ringBell() const;
    void emergencyRedirect(); //Method used in emergencies such as fire or power out to redirect car to safe floor
};

#endif // ELEVATOR_H
