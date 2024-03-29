#include "floor.h"

const char Floor::GROUND_LEVEL = 0;

Floor::Floor(int level, bool top)
    : level(level)
    , top(top)
    , waitingUp(false)
    , waitingDown(false)
{ }

//getters
int Floor::getLevel() const { return level; }
bool Floor::isTop() const { return top; }
bool Floor::isWaitingUp() const { return waitingUp; }
bool Floor::isWaitingDown() const { return waitingDown; }

void Floor::inform(Direction direction) {
    if(direction == Direction::UP && waitingUp == false){
        waitingUp = true;
        emit requestForElevator(level, Direction::UP);
    } else if(direction == Direction::DOWN  && waitingDown == false){
        waitingDown = true;
        emit requestForElevator(level, Direction::DOWN); 
    }

}

void Floor::serviced(Direction direction) {
    if(direction == Direction::UP) waitingUp = false;
    if(direction == Direction::DOWN) waitingDown = false;
    emit floorServiced();
}

