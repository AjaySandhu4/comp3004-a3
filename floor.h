#ifndef FLOOR_H
#define FLOOR_H

#include <QObject>
#include <QDebug>
#include <QTextStream>
#include <iostream>
#include "direction.h"

class Floor : public QObject
{
    Q_OBJECT
public:
    Floor(int level, bool top);
    friend std::ostream& operator<< (std::ostream &os, const Floor &f) {
        return (os << "Floor number: " << f.level << "\n Top: " << f.top << std::endl);
    }

    int getLevel() const;
    bool isTop() const;
    bool isWaitingUp() const;
    bool isWaitingDown() const;

    const static char GROUND_LEVEL;

public slots:
    void inform(Direction direction);

private:
    int level; //Number of the floor
    bool top; //Is this the top floor
    bool waitingUp; // Is there a request from this floor in the "up" direction
    bool waitingDown; // Is there a request from this floor in the "down" direction
};

#endif // FLOOR_H
