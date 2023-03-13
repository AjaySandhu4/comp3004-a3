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
    if(direction == Direction::UP){
        waitingUp = true;
        QTextStream(stdout) << "Floor " << level << " being informed in direction UP" << endl;
//        std::cout << "Floor " << level << " being informed in direction UP" << std::endl;
    } else if(direction == Direction::DOWN){
        waitingDown = true;
        QTextStream(stdout) << "Floor " << level << " being informed in direction DOWN" << endl;
    } else {
        std::cerr << "Can only inform floor of UP or DOWN" << std::endl;
    }

}

