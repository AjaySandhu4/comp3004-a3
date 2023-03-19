#ifndef ELEVATORFLOORSENSOR_H
#define ELEVATORFLOORSENSOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include "direction.h"

// Purpose of class: Used to inform Elevator of upcoming floors
class ElevatorFloorSensor : public QObject
{
    Q_OBJECT
public:
    ElevatorFloorSensor();

public slots:
    void detectMovement(int startingFloor, Direction direction); //Sensor uses this to begin timer until next floor

private slots:
    void onTimeout(); //When timer timeouts, a new floor has been reached

signals:
    void detectedFloor(int floorNum); //Used to inform elevator of new floor

private:
    const static int INTERVAL_BETWEEN_FLOORS; //Time it takes for the elevator to reach one floor to the next
    QTimer timer;
    int nextFloor; //The approaching floor

};

#endif // ELEVATORFLOORSENSOR_H
