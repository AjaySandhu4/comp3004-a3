#ifndef FLOOR_H
#define FLOOR_H

#include <iostream>

class Floor
{
public:
    Floor(int level, bool top);
    friend std::ostream& operator<< (std::ostream &os, const Floor &f) {
        return (os << "Floor number: " << f.level << "\n Top: " << f.top << std::endl);
    }

    int getLevel() const;
    bool isTop() const;
    bool isWaitingUp() const;
    bool isWaitingDown() const;

private:
    int level; //Number of the floor
    bool top; //Is this the top floor
    bool waitingUp; // Is there a request from this floor in the "up" direction
    bool waitingDown; // Is there a request from this floor in the "down" direction
};

#endif // FLOOR_H
