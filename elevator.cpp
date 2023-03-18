#include "elevator.h"

const int Elevator::DOOR_TIMER_INTERVAL = 8000; //Takes 8 seconds for elevator to try and close its doors
const int Elevator::TIME_DOORS_TAKE_TO_SHUT = 2500; //Takes 2.5 seconds from elevator telling doors to close to them actually being shut
const char Elevator::REQUESTED_UP = 0b10; //Bitmask that represents that a floor request exists in the UP direction
const char Elevator::REQUESTED_DOWN = 0b01; //Bitmask that represents that a floor request exists in the DOWN direction

Elevator::Elevator(int carNum, int numFloors)
    : carNum(carNum)
    , numFloors(numFloors)
    , currFloor(0)
    , state(ElevatorState::IDLE)
    , floorRequests(new char[numFloors]{0})
    , doorsOpen(true)
    , weightSensor(new WeightSensor())
    , doorLightSensor(new DoorLightSensor())
    , direction(Direction::UP)
    , consecutiveDoorInterruptions(0)
{
    connect(this, &Elevator::moving, &floorSensor, &ElevatorFloorSensor::detectMovement);
    connect(&floorSensor, &ElevatorFloorSensor::detectedFloor, this, &Elevator::newFloor);

    connect(weightSensor, &WeightSensor::overload, this, &Elevator::handleOverload);

    connect(doorLightSensor, &DoorLightSensor::triggered, this, &Elevator::handleDoorObstacle);

    connect(&doorTimer, &QTimer::timeout, this, &Elevator::doorTimerFinished);
    connect(&closeDoorsTimer, &QTimer::timeout, this, &Elevator::doorsHaveShut);

    doorTimer.start(DOOR_TIMER_INTERVAL);
}

Elevator::~Elevator()
{
    delete weightSensor;
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
WeightSensor* Elevator::getWeightSensor() const { return weightSensor; }
bool Elevator::areDoorsOpen() const { return doorsOpen; }
bool Elevator::isEmpty() const { return !weightSensor->isLoaded(); }

bool Elevator::areFloorRequestsEmpty() const {
    for(int i=0; i<numFloors; ++i){
        if(isRequestedFloor(i)) return false;
    }
    return true;
}

void Elevator::destFloorRequest(int requestFloor, Direction requestDirection)
{
    if(requestFloor == currFloor && (state == ElevatorState::WAITING || state == ElevatorState::IDLE)){ //Already in middle of servicing requested floor
        emit floorServiced(currFloor, direction);
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
    openDoors();
    QTextStream(stdout) << "Elevator " << carNum << ": Servicing floor " << currFloor <<"..."<< endl;

    //Remove floor request for current floor
    if(direction == Direction::DOWN) floorRequests[currFloor] = floorRequests[currFloor] & ~REQUESTED_DOWN;
    else if(direction == Direction::UP) floorRequests[currFloor] = floorRequests[currFloor] & ~REQUESTED_UP;

    emit floorServiced(currFloor, direction);

    if(state == ElevatorState::OUT_OF_SERVICE){
        // Audio and text telling passengers to disembark as the elevator was put out of service (e.g. through a power outage or fire)
        voice("The elevator has reached a safe floor... Please disembark!");
        textMessage("Safe floor reached. Please disembark safely.");
    } else {
        state = areFloorRequestsEmpty() ? ElevatorState::IDLE : ElevatorState::WAITING;
        doorTimer.start(DOOR_TIMER_INTERVAL);
    }

}

void Elevator::closeDoors()
{
    ringBell();
    closeDoorsTimer.start(TIME_DOORS_TAKE_TO_SHUT);
    doorLightSensor->setActive(true);
    QTextStream(stdout) << "Elevator " << carNum << ": Doors are closing..." << endl;
//    emit doorsClosing();
}

void Elevator::openDoors()
{
    ringBell();
    doorsOpen = true;
    QTextStream(stdout) << "Elevator " << carNum << ": Doors are opening..." << endl;
}

void Elevator::move()
{
    state = ElevatorState::MOVING;
    emit moving(currFloor, direction);
    QTextStream(stdout) << "Elevator " << carNum << ": On the move from floor " << currFloor << " in the direction " << direction << "..." << endl;
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

    if(weightSensor->overloadedBy() > 0){
        handleOverload();
    } else if(state == ElevatorState::IDLE){ //If still idle (no floor requests received), restart the timer (elevator won't close doors and move)
        doorTimer.start(DOOR_TIMER_INTERVAL);
    } else{
        closeDoors();
    }
}

void Elevator::doorsHaveShut() {
    closeDoorsTimer.stop();
    doorLightSensor->setActive(false);
    consecutiveDoorInterruptions = 0;
    QTextStream(stdout) << "Elevator " << carNum << ": Doors have shut..." << endl;
    doorsOpen = false;
    move();
}

void Elevator::handleOverload() {
    doorTimer.stop();
    //TODO: use audio system and display...
    QString overload = "Elevator " + QString::number(carNum) +" is overloaded!";
    voice(overload);
    textMessage(overload);
    qWarning() << "Elevator " << carNum << " is overloaded by " << weightSensor->overloadedBy() << "kg!" << endl;
    doorTimer.start(DOOR_TIMER_INTERVAL);
}

void Elevator::handleDoorObstacle() {
    closeDoorsTimer.stop();
    ++consecutiveDoorInterruptions;
    qWarning() << "Elevator " << carNum << " has encountered a door obstacle!" << endl;
    if(consecutiveDoorInterruptions > 1){
        qWarning() << "Elevator " << carNum << " has encountered repeated door obstacles. Please do not get in way of doors as they close!" << endl;
    }
}

void Elevator::handleFire() {
    memset(floorRequests, 0, numFloors); //Cancel all destination floor requests on current elevator
    state = ElevatorState::OUT_OF_SERVICE;
    direction = Direction::DOWN;
    floorRequests[0] = REQUESTED_UP;
    closeDoors();
}

void Elevator::ringBell() {
    QTextStream(stdout) << "Elevator " << carNum << ": *Riiiiinnnnnngggg*" << endl;
}

void Elevator::onOpenButtonPress() {
    if(doorTimer.isActive()){
        doorTimer.start(DOOR_TIMER_INTERVAL); //If door timer is still running, restart its timer (which extends the interval they are open)
        QTextStream(stdout) << "Elevator " << carNum << ": Open door button pressed... Doors being left open for longer..." << endl;
    } else if(closeDoorsTimer.isActive()){ //If doors were closing when the button was pressed, cancel the closing of the doors and restart door time
        closeDoorsTimer.stop();
        doorTimer.start(DOOR_TIMER_INTERVAL);
        QTextStream(stdout) << "Elevator " << carNum << ": Open button pressed... Door closing cancelled, reopening doors..." << endl;
    } else {
        QTextStream(stdout) << "Elevator " << carNum << ": Open button pressed, but doors are not open at this time..." << endl;
    }
}

void Elevator::onCloseButtonRelease() {
    if(doorTimer.isActive()){ //If door timer is still running, cut remaining time in half
        doorTimer.start(doorTimer.remainingTime()/2);
        QTextStream(stdout) << "Elevator " << carNum << ": Close button released... Time before doors close reduced..." << endl;
    } else {
        QTextStream(stdout) << "Elevator " << carNum << ": Close button pressed, but doors cannot be forced closed at this time..." << endl;
    }
}
