#include "safetyservice.h"

SafetyService::SafetyService()
{ }

void SafetyService::connectWithCar(Elevator* car) {
    connect(this, SIGNAL(responded(const QString&)), car, SIGNAL(voice(const QString&)));

    // Sends response in random time between 0 and 8 seconds (if response is in more than 5 seconds it won't be in time)
    QTimer::singleShot(QRandomGenerator::global()->bounded(8000), this, &SafetyService::sendResponse);
}

void SafetyService::sendResponse(){
    emit responded("This is the safety service response");
}
