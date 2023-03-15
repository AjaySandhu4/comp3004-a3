#include "elevator.h"

const int Elevator::DOOR_TIMER_INTERVAL = 10000;
const int Elevator::TIME_DOORS_TAKE_TO_SHUT = 2500;
const char Elevator::REQUESTED_UP = 0b10;
const char Elevator::REQUESTED_DOWN = 0b01;

Elevator::Elevator(int carNum, int numFloors)
    : carNum(carNum)
    , numFloors(numFloors)
    , currFloor(0)
    , floorRequests(new char[numFloors]{0})
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

bool Elevator::isRequestedFloor(int floorNum, Direction requestDirection) const {
    if(requestDirection == Direction::UP) return floorRequests[floorNum] & REQUESTED_UP;
    if(requestDirection == Direction::DOWN) return floorRequests[floorNum] & REQUESTED_DOWN;
    return floorRequests[floorNum];
}

int Elevator::getCurrFloor() const { return currFloor; }
int Elevator::getCarNum() const { return carNum; }
ElevatorState Elevator::getState() const { return state; }
Direction Elevator::getDirection() const { return direction; }

bool Elevator::areFloorRequestsEmpty() const {
    for(int i=0; i<numFloors; ++i){
        if(isRequestedFloor(i)) return false;
    }
    return true;
}

void Elevator::destFloorRequest(int requestFloor, Direction requestDirection)
{
    QTextStream(stdout) << "Car number " << carNum << " has received request for floor " << requestFloor << " in direction " << requestDirection << endl;
    if(requestFloor == currFloor && (state == ElevatorState::WAITING || state == ElevatorState::IDLE)){ //Already in middle of servicing requested floor
        qDebug() << "Floor is being serviced immediately on request";
        emit floorServiced(currFloor, direction);
        qDebug() << "Is it getting here?";
        return;
    }
    if(requestDirection == Direction::UNKNOWN){
        if(requestFloor == numFloors-1){
            requestDirection = Direction::DOWN;
        } else if(requestFloor == 0){
            requestDirection = Direction::UP;
        } else if(direction == Direction::UP){
            requestDirection = currFloor - requestFloor < 0 ? Direction::UP : Direction::DOWN;
        } else if(direction == Direction::DOWN){
            requestDirection = requestFloor - currFloor < 0 ? Direction::DOWN : Direction::UP;
        }
    }
    //Add the floor request
    if(requestDirection == Direction::UP) floorRequests[requestFloor] = floorRequests[requestFloor] | REQUESTED_UP;
    if(requestDirection == Direction::DOWN) floorRequests[requestFloor] = floorRequests[requestFloor] | REQUESTED_DOWN;

    //Change state if necessary
    if(state == ElevatorState::IDLE){
        state = ElevatorState::WAITING;
        decideDirectionToGo(); //The first floor request determines the direction the car will travel
    }
}

void Elevator::newFloor(int floorNum)
{
    currFloor = floorNum;
    emit reachedFloor(currFloor);

    std::cout << "Car number " << carNum << " has reached floor " << currFloor << std::endl;

    //Change direction if car has reached the top floor or ground floor
    if(currFloor == 0 || currFloor == numFloors-1){
        direction = currFloor == 0 ? Direction::UP : Direction::DOWN;
    }
    if(isRequestedFloor(currFloor, direction)){
        stop();
    } else{
        move();
    }
}

void Elevator::stop()
{
    QTextStream(stdout) << "Elevator " << carNum << " is servicing floor " << currFloor << endl;
    doorsOpen = true;

    //Remove floor request for current floor
    if(direction == Direction::DOWN) floorRequests[currFloor] = floorRequests[currFloor] & ~REQUESTED_DOWN;
    else if(direction == Direction::UP) floorRequests[currFloor] = floorRequests[currFloor] & ~REQUESTED_UP;

    emit floorServiced(currFloor, direction);
    state = areFloorRequestsEmpty() ? ElevatorState::IDLE : ElevatorState::WAITING;
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
    QTextStream(stdout) << "Elevator " << carNum << " is on the move from floor " << currFloor << " in the direction " << direction << endl;
}

// Check if there are requests in same direction elevator was moving and otherwise change direction
void Elevator::decideDirectionToGo()
{
    if(direction == Direction::UP){
        for(int i=currFloor+1; i<numFloors; ++i){
            if(isRequestedFloor(i)) return;
        }
        direction = Direction::DOWN;
    }
    else if(direction == Direction::DOWN){
        for(int i=currFloor-1; i>=0; --i){
            if(isRequestedFloor(i)) return;
        }
        direction = Direction::UP;
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
