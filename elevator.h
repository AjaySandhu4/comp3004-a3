#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QVector>
#include <iostream>

class Elevator : public QObject
{
    Q_OBJECT
public:
    Elevator(int carNum, int numFloors);
    ~Elevator();

    friend std::ostream& operator<< (std::ostream &os, const Elevator &e) {
        return (os << "Car number: " << e.carNum << "\nCurrent floor: " << e.currFloor << std::endl);
    }

    bool isFloorRequest(int floorNum) const;

public slots:
    void destFloorRequest(int floorNum);
    void newFloor(int floorNum);

private:
    int carNum;
    int currFloor;
    bool *floorRequests;

//signals:

};

#endif // ELEVATOR_H
