#ifndef FLOOR_H
#define FLOOR_H

#include <QObject>
#include <QDebug>
#include <QTextStream>
#include <iostream>
#include "direction.h"

//Purpose of class: Represents a floor within the building
class Floor : public QObject
{
    Q_OBJECT
public:
    Floor(int level, bool top);

    int getLevel() const;
    bool isTop() const;
    bool isWaitingUp() const;
    bool isWaitingDown() const;

    void serviced(Direction direction); //Called to inform floor that it has been serviced in a particular direction

    const static char GROUND_LEVEL; //The ground level floor number


public slots:
    void inform(Direction direction); //Invoked to inform floor that there is a request in a particular direction

signals:
    void requestForElevator(int floorNum, Direction direction); // Signal used to ask the ECS for an elevator to service the floor in a particular direction
    void floorServiced(); // Signal emitted to signify that the floor has been serviced

private:
    const int level; //Number of the floor
    const bool top; //Is this the top floor
    bool waitingUp; // Is there a request from this floor in the "up" direction
    bool waitingDown; // Is there a request from this floor in the "down" direction
};

#endif // FLOOR_H
