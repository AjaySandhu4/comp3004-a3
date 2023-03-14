#include "elevator.h"

const int Elevator::DOOR_TIMER_INTERVAL = 10000;
const int Elevator::TIME_DOORS_TAKE_TO_SHUT = 2500;

Elevator::Elevator(int carNum, int numFloors)
    : carNum(carNum)
    , numFloors(numFloors)
    , currFloor(0)
    , floorRequests(new bool[numFloors]{0})
    , doorsOpen(true)
    , state(ElevatorState::IDLE)
{
    connect(this, &Elevator::moving, &floorSensor, &ElevatorFloorSensor::detectMovement);
    connect(&floorSensor, &ElevatorFloorSensor::detectedFloor, this, &Elevator::newFloor);

    connect(&doorTimer, &QTimer::timeout, this, &Elevator::doorTimerFinished);
    connect(&closeDoorsTimer, &QTimer::timeout, this, &Elevator::doorsHaveShut);

    doorTimer.start(DOOR_TIMER_INTERVAL);
}

Elevator::~Elevator()
{
    delete floorRequests;
}

bool Elevator::isRequestedFloor(int floorNum) const { return floorRequests[floorNum]; }
int Elevator::getCurrFloor() const { return currFloor; }

bool Elevator::isFloorRequestsEmpty() const {
    for(int i=0; i<numFloors; ++i){
        if(isRequestedFloor(i)) return false;
    }
    return true;
}

void Elevator::destFloorRequest(int floorNum)
{
    if(floorNum == currFloor && (state == ElevatorState::WAITING || state == ElevatorState::IDLE)) return;
    floorRequests[floorNum] = true;
    std::cout << "Car number " << carNum << " has received request for floor " << floorNum << std::endl;
}

void Elevator::newFloor(int floorNum)
{
    currFloor = floorNum;
    emit reachedFloor(currFloor);
    std::cout << "Car number " << carNum << " has reached floor " << currFloor << std::endl;
    if(isRequestedFloor(floorNum)){
        stop();
    }
    else{
        move();
    }
}

void Elevator::stop()
{
    QTextStream(stdout) << "Elevator " << carNum << " is servicing floor " << currFloor << endl;
    doorsOpen = true;
    state = ElevatorState::WAITING;
    floorRequests[currFloor] = 0; //Remove floor request for current floor
    emit floorServiced(currFloor);
    doorTimer.start(DOOR_TIMER_INTERVAL);
}

void Elevator::closeDoors()
{
    closeDoorsTimer.start(TIME_DOORS_TAKE_TO_SHUT);
}

void Elevator::move()
{
    direction = directionToGo();
    state = ElevatorState::MOVING;
    emit moving(currFloor, direction);
    QTextStream(stdout) << "Elevator " << carNum << " is on the move from floor " << currFloor << endl;
}

// Check if there are requests in same direction elevator was moving and otherwise return opposite direction
Direction Elevator::directionToGo() const
{
    if(direction == Direction::UP){
        for(int i=currFloor+1; i<numFloors; ++i){
            if(isRequestedFloor(i)) return Direction::UP;
        }
        return Direction::DOWN;
    }
    else if(direction == Direction::DOWN){
        for(int i=currFloor-1; i>=0; --i){
            if(isRequestedFloor(i)) return Direction::DOWN;
        }
    }
    return Direction::UP;
}

void Elevator::doorTimerFinished() {
    doorTimer.stop();
    //If no more floor requests, set car to being idle and restart door timer...
    if(isFloorRequestsEmpty()){
        state = ElevatorState::IDLE;
        doorTimer.start(DOOR_TIMER_INTERVAL);
    }
    else{
        closeDoors();
    }
}

void Elevator::doorsHaveShut() {
    closeDoorsTimer.stop();
    QTextStream(stdout) << "Elevator " << carNum << " door's have shut at floor " << currFloor << "..." << endl;
    doorsOpen = false;
    move();
}
