#include "floor.h"

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

