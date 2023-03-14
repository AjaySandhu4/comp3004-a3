#include "floor.h"

const char Floor::GROUND_LEVEL = 0;

Floor::Floor(int level, bool top)
    : level(level)
    , top(top)
    , waitingUp(false)
    , waitingDown(false)
{ }

int Floor::getLevel() const { return level; }
bool Floor::isTop() const { return top; }
bool Floor::isWaitingUp() const { return waitingUp; }
bool Floor::isWaitingDown() const { return waitingDown; }

void Floor::inform(Direction direction) {
    if(direction == Direction::UP && waitingUp == false){
        waitingUp = true;
        emit requestForElevator(level, Direction::UP);
        QTextStream(stdout) << "Floor " << level << " being informed in direction UP" << endl;
    } else if(direction == Direction::DOWN  && waitingDown == false){
        waitingDown = true;
        emit requestForElevator(level, Direction::DOWN);
        QTextStream(stdout) << "Floor " << level << " being informed in direction DOWN" << endl;
    }

}

void Floor::serviced(Direction direction) {
//    QString d = direction == Direction::UP ? "UP" : "DOWN";
    QTextStream(stdout) << "Floor " << level << " being serviced in direction " << direction << "!" << endl;
    if(direction == Direction::UP) waitingUp = false;
    if(direction == Direction::DOWN) waitingDown = false;
    emit floorServiced();
}

