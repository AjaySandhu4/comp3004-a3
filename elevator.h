#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>

class Elevator : public QObject
{
    Q_OBJECT
public:
    Elevator(int carNum);

    friend std::ostream& operator<< (std::ostream &os, const Elevator &e) {
        return (os << "Car number: " << e.carNum << "\nCurrent floor: " << e.currentFloor << std::endl);
    }

private:
    int carNum;
    int currentFloor;

signals:

};

#endif // ELEVATOR_H
