#include "timedependentstrategy.h"

void TimeDependentStrategy::allocateElevator(int requestFloor, Direction requestDirection, QVector<Elevator*> *cars, int totalFloors)
{
    //If time is NOT between the designated busy times of 7am-11am or 4pm-7pm, use the default allocation algorithm
    int currentHour = QTime::currentTime().hour();
    if(!((currentHour>=7 && currentHour<=11) || (currentHour>=16 && currentHour<=17))){
        ElevatorCenteredStrategy::allocateElevator(requestFloor, requestDirection, cars, totalFloors);
        return;
    }

    //If elevator is in middle of servicing the floor in the request direction or is idle at that floor, allocate the request to the servicing elevator...
    for(int i=0; i<cars->length(); ++i){
        if((cars->value(i)->getCurrFloor() == requestFloor) && (cars->value(i)->getState() == ElevatorState::IDLE || (cars->value(i)->getDirection() == requestDirection && cars->value(i)->getState() == ElevatorState::WAITING))){
            cars->value(i)->destFloorRequest(requestFloor, requestDirection);
            return;
        }
    }

    //Prioritizes floor requests in the busy hours by providing those floors with a previously idle elevator (if possible)
    for(int i=0; i<cars->length(); ++i){
        if(cars->value(i)->getState() == ElevatorState::IDLE){
            cars->value(i)->destFloorRequest(requestFloor, requestDirection);
            return;
        }
    }

    //If Idle elevator can't be found, just find the closest elevator (ignoring the fact that an elevator might already have the requested floor on its request list)
    int closestDistance = 2 * cars->length();
    Elevator* closestElevator = cars->value(0);
    for(int i=0; i<cars->length(); ++i){
        Elevator* currElevator = cars->value(i);
        int currElevatorDistance = 2 * cars->length();

        /* Elevator direction and request direction can have 4 combinations -> (UP,UP), (UP, DOWN), (DOWN, UP), (DOWN, DOWN) */
        //Elevator going UP and request is UP
        if(currElevator->getDirection() == Direction::UP && requestDirection == Direction::UP){
            //Use equation based on whether elevator is lower then request floor
            currElevatorDistance = currElevator->getCurrFloor() < requestFloor
                    ? requestFloor - currElevator->getCurrFloor()
                    : 2*totalFloors + requestFloor - currElevator->getCurrFloor() - 2;
        } else if(currElevator->getDirection() == Direction::DOWN && requestDirection == Direction::DOWN){
            //Use equation based on whether request floor is lower than elevator
            currElevatorDistance = requestFloor < currElevator->getCurrFloor()
                    ? currElevator->getCurrFloor() - requestFloor
                    : 2*totalFloors - requestFloor + currElevator->getCurrFloor() - 2;
        } else if(currElevator->getDirection() == Direction::UP && requestDirection == Direction::DOWN){
            currElevatorDistance = 2*totalFloors - currElevator->getCurrFloor() - requestFloor - 2;
        } else {
            currElevatorDistance = currElevator->getCurrFloor() + requestFloor;
        }

        //Update closest elevator and distance if necessary
        if(currElevatorDistance < closestDistance){
            closestDistance = currElevatorDistance;
            closestElevator = currElevator;
        }
    }
    //Allocate the request to the elevator determined to be the closest
    closestElevator->destFloorRequest(requestFloor, requestDirection);



}
