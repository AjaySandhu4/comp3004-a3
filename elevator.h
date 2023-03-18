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


class Elevator : public QObject
{
    Q_OBJECT
public:
    Elevator(int carNum, int numFloors);
    ~Elevator();

    friend std::ostream& operator<< (std::ostream &os, const Elevator &e) {
        return (os << "Car number: " << e.carNum << "\nCurrent floor: " << e.currFloor << std::endl);
    }

    bool isRequestedFloor(int floorNum, Direction requestDirection = Direction::UNKNOWN) const;
    int getCurrFloor() const;
    int getCarNum() const;
    ElevatorState getState() const;
    Direction getDirection() const;
    bool isEmpty() const;
    WeightSensor* getWeightSensor() const;
    bool areDoorsOpen() const;
    void onOpenButtonPress();
    void onCloseButtonRelease();

public slots:
    void destFloorRequest(int floorNum, Direction requestDirection = Direction::UNKNOWN);
    void newFloor(int floorNum);
    void handleOverload();
    void handleDoorObstacle();
    void handleFire();

private slots:
    void doorsHaveShut();
    void doorTimerFinished();
    void move();

signals:
    void moving(int startingFloor, Direction direction);
    void floorServiced(int floorNum, Direction direction);
    void reachedFloor(int floorNum);
    void doorsClosing();
    void voice(const QString&);
    void textMessage(const QString&);

private:
    const int carNum;
    const int numFloors;
    int currFloor;
    char *floorRequests;
    bool doorsOpen;
    ElevatorState state;
    Direction direction;
    ElevatorFloorSensor floorSensor;
    DoorLightSensor *doorLightSensor;
    WeightSensor *weightSensor;
    QTimer doorTimer; // Timer used to keep doors open to allow passengers to enter and exit
    QTimer closeDoorsTimer; // Timer used to keep track of when doors are in process of closing
    int consecutiveDoorInterruptions;

    const static int DOOR_TIMER_INTERVAL;
    const static int TIME_DOORS_TAKE_TO_SHUT;
    const static char REQUESTED_UP;
    const static char REQUESTED_DOWN;

    void stop();
    void closeDoors();
    void openDoors();
    void decideDirectionToGo();
    bool areFloorRequestsEmpty() const;
    void ringBell();

//signals:

};

#endif // ELEVATOR_H
