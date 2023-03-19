#include "ecs.h"

const int ECS::MAX_HELP_RESPONSE_TIME = 5000; //Max is 5 seconds before 911 is called in a signal for help situation

ECS::ECS(AllocationStrategy* strategy, QVector<Floor*> *floors, QVector<Elevator*> *elevators)
    : floors(floors)
    , elevators(elevators)
    , strategy(strategy)
    , safetyService(new SafetyService)
    , helpResponseReceived(false)
    , safetyResponseReceived(false)
    , carGettingHelp(nullptr)
{
    for(int i=0; i<floors->length(); ++i){
        connect(floors->value(i), &Floor::requestForElevator, this, &ECS::allocateElevator);
    }
    for(int i=0; i<elevators->length(); ++i){
        connect(elevators->value(i), &Elevator::floorServiced, this, &ECS::floorServiced);
        connect(elevators->value(i), SIGNAL(callingForHelp(Elevator*)), this, SLOT(handleHelp(Elevator*)));
        connect(this, &ECS::fire, elevators->value(i), &Elevator::handleFire);
        connect(this, &ECS::powerOut, elevators->value(i), &Elevator::handlePowerOut);
    }
}

ECS::~ECS()
{
    delete safetyService;
}

void ECS::allocateElevator(int requestFloor, Direction requestDirection)
{
    strategy->allocateElevator(requestFloor, requestDirection, elevators, floors->length());
}

void ECS::floorServiced(int floorNum, Direction direction)
{
    floors->value(floorNum)->serviced(direction);
}

void ECS::handleFire(){
    QTextStream(stdout) << "ECS: Signalled of a fire in the building!" << endl;
    emit fire();
}

void ECS::handlePowerOut(){
    QTextStream(stdout) << "ECS: Signalled of a power outage in the building!" << endl;
    emit powerOut();
}

void ECS::handleHelp(Elevator* car)
{
    carGettingHelp = car;
    safetyService->connectWithCar(carGettingHelp);
    connect(safetyService, &SafetyService::responded, this, &ECS::receiveSafetyResponse);
    connect(carGettingHelp, &Elevator::helpFollowUp, this, &ECS::receiveHelpResponse);
    QTimer::singleShot(MAX_HELP_RESPONSE_TIME, this, &ECS::onHelpTimerTimeout); //Begin timer to make sure responses are in time
}

void ECS::onHelpTimerTimeout()
{
    disconnect(carGettingHelp, &Elevator::helpFollowUp, this, &ECS::receiveHelpResponse);
    disconnect(safetyService, nullptr, nullptr, nullptr);
    if(helpResponseReceived && safetyResponseReceived){
        QTextStream(stdout) << "ECS: Elevator " << carGettingHelp->getCarNum() << " was given help from the safety service!" << endl;
    } else if(!safetyResponseReceived) {
        QTextStream(stdout) << "ECS: Elevator " << carGettingHelp->getCarNum() << " was not given help from the safety service!" << endl;
        call911();
    } else if(!helpResponseReceived) {
        QTextStream(stdout) << "ECS: Elevator " << carGettingHelp->getCarNum() << " did not follow up on their call for help from the safety service!" << endl;
        call911();
    }
}

void ECS::call911() const
{
    QTextStream(stdout) << "ECS: Placing a call for 911 for elevator " << carGettingHelp->getCarNum() << "!" << endl;
}
void ECS::receiveHelpResponse(){ helpResponseReceived = true; }
void ECS::receiveSafetyResponse(){ safetyResponseReceived = true; }
