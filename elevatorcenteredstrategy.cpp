#include "elevatorcenteredstrategy.h"

void ElevatorCenteredStrategy::allocateElevator(int requestFloor, Direction requestDirection, QVector<Elevator*> *cars, int totalFloors)
{
    //Check if elevator already contains the same floor request, simply do nothing and return...
    for(int i=0; i<cars->length(); ++i){
        if(cars->value(i)->isRequestedFloor(requestFloor, requestDirection)) return;
    }

    //Otherwise, pick the closest car...
    int closestDistance = 2 * cars->length();
    Elevator* closestElevator = cars->value(0);
    for(int i=0; i<cars->length(); ++i){
        Elevator* currElevator = cars->value(i);
        int currElevatorDistance = 2 * cars->length();

        //If elevator is in middle of servicing the floor in the request direction or is idle at that floor, allocate the request to the servicing elevator...
        if((currElevator->getCurrFloor() == requestFloor) && (currElevator->getState() == ElevatorState::IDLE || (currElevator->getDirection() == requestDirection && currElevator->getState() == ElevatorState::WAITING))){
            currElevator->destFloorRequest(requestFloor, requestDirection);
            return;
        }

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
