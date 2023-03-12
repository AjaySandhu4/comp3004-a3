#include "floor.h"

const char Floor::UP = 1;
const char Floor::DOWN = 0;


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
    std::cout << "Floor being informed in direction " << direction << std::endl;
}

