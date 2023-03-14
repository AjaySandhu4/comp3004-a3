#include "elevator.h"

const int Elevator::DOOR_TIMER_INTERVAL = 10000;
const int Elevator::TIME_DOORS_TAKE_TO_SHUT = 2500;
const char Elevator::REQUESTED_UP = 0b10;
const char Elevator::REQUESTED_DOWN = 0b01;

Elevator::Elevator(int carNum, int numFloors)
    : carNum(carNum)
    , numFloors(numFloors)
    , currFloor(0)
    , floorRequests(new bool[numFloors]{0})
    , doorsOpen(true)
    , state(ElevatorState::IDLE)
    , direction(Direction::UP)
{
    connect(this, &Elevator::moving, &floorSensor, &ElevatorFloorSensor::detectMovement);
    connect(&floorSensor, &ElevatorFloorSensor::detectedFloor, this, &Elevator::newFloor);

    connect(&doorTimer, &QTimer::timeout, this, &Elevator::doorTimerFinished);
    connect(&closeDoorsTimer, &QTimer::timeout, this, &Elevator::doorsHaveShut);

    doorTimer.start(DOOR_TIMER_INTERVAL);
}

Elevator::~Elevator()
{
    delete[] floorRequests;
}

bool Elevator::isRequestedFloor(int floorNum) const { return floorRequests[floorNum]; }
int Elevator::getCurrFloor() const { return currFloor; }
int Elevator::getCarNum() const { return carNum; }

bool Elevator::areFloorRequestsEmpty() const {
    for(int i=0; i<numFloors; ++i){
        if(isRequestedFloor(i)) return false;
    }
    return true;
}

void Elevator::destFloorRequest(int floorNum)
{
    if(floorNum == currFloor && (state == ElevatorState::WAITING || state == ElevatorState::IDLE)){ //Already in middle of servicing requested floor
        emit floorServiced(currFloor, direction);
    } else {
        floorRequests[floorNum] = true; //Add the floor request
        if(state == ElevatorState::IDLE){
            state = ElevatorState::WAITING;
            decideDirectionToGo(); //The first floor request determines the direction the car will travel
        }
    }
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
    floorRequests[currFloor] = 0; //Remove floor request for current floor
    emit floorServiced(currFloor, direction);
    if(areFloorRequestsEmpty()){
        state = ElevatorState::IDLE;
        emit nowIdle(carNum);
    } else {
        state = ElevatorState::WAITING;
    }
    doorTimer.start(DOOR_TIMER_INTERVAL);
}

void Elevator::closeDoors()
{
    closeDoorsTimer.start(TIME_DOORS_TAKE_TO_SHUT);
}

void Elevator::move()
{
    state = ElevatorState::MOVING;
    emit moving(currFloor, direction);
    QTextStream(stdout) << "Elevator " << carNum << " is on the move from floor " << currFloor << endl;
}

// Check if there are requests in same direction elevator was moving and otherwise return opposite direction
void Elevator::decideDirectionToGo()
{
    if(direction == Direction::UP){
        for(int i=currFloor+1; i<numFloors; ++i){
            if(isRequestedFloor(i)) return;
        }
        direction = Direction::DOWN;
        emit changedDirection(carNum);
    }
    else if(direction == Direction::DOWN){
        for(int i=currFloor-1; i>=0; --i){
            if(isRequestedFloor(i)) return;
        }
        direction = Direction::UP;
        emit changedDirection(carNum);
    }
}

void Elevator::doorTimerFinished() {
    doorTimer.stop();
    //If still idle (no floor requests received), restart the timer...
    if(state == ElevatorState::IDLE){
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
