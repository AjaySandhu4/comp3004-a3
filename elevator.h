#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <iostream>

enum ElevatorState {
    MOVINGUP,
    MOVINGDOWN,
    WAITING,
    BLOCKED,
    OUTOFSERVICE,
    IDLE
};

class Elevator : public QObject
{
    Q_OBJECT
public:
    Elevator(int carNum, int numFloors);
    ~Elevator();

    friend std::ostream& operator<< (std::ostream &os, const Elevator &e) {
        return (os << "Car number: " << e.carNum << "\nCurrent floor: " << e.currFloor << std::endl);
    }

    bool isRequestedFloor(int floorNum) const;

public slots:
    void destFloorRequest(int floorNum);
    void newFloor(int floorNum);

signals:
    void moving(int startingFloor, ElevatorState state);

private:
    int carNum;
    int currFloor;
    bool *floorRequests;
    bool doorsOpen;
    ElevatorState state;

//signals:

};

#endif // ELEVATOR_H
