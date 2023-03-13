#ifndef ELEVATORFLOORSENSOR_H
#define ELEVATORFLOORSENSOR_H

#include <QObject>
#include <QTimer>
#include "elevator.h"

class ElevatorFloorSensor : public QObject
{
    Q_OBJECT
public:
    ElevatorFloorSensor();

public slots:
    void detectMovement(int startingFloor, ElevatorState state);

private slots:
    void onTimeout();

signals:
    void detectedFloor(int floorNum);

private:
    const static int INTERVAL_BETWEEN_FLOORS;
    QTimer timer;

};

#endif // ELEVATORFLOORSENSOR_H
