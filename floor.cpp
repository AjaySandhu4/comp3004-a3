#include "floor.h"

const char Floor::UP = 1;
const char Floor::DOWN = 0;

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

void Floor::inform(char direction) {
    if(direction == UP){
        waitingUp = true;
        std::cout << "Floor " << level << " being informed in direction UP" << std::endl;
    } else if(direction == DOWN){
        waitingDown = true;
        std::cout << "Floor " << level << " being informed in direction DOWN" << std::endl;
    } else {
        std::cerr << "Can only inform floor of UP or DOWN" << std::endl;
    }

}

