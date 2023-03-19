#ifndef ELEVATORSTATE_H
#define ELEVATORSTATE_H

//Used to represent the states that an elevator can be in
enum class ElevatorState {
    MOVING, //Is on the move to another floor
    WAITING, //Is servicing a floor but still has more requests left
    IDLE //Is servicing a floor but has no more requests left
};

#endif // ELEVATORSTATE_H
