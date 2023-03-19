#ifndef SAFETYSERVICE_H
#define SAFETYSERVICE_H

#include <QObject>
#include <QTimer>
#include <QRandomGenerator>
#include "elevator.h"

// Purpose of class: Sends voice response to an Elevator when it is given a "Help" signal
class SafetyService : public QObject
{
    Q_OBJECT
public:
    SafetyService();

public slots:
    void connectWithCar(Elevator* car); //Sends response to car specified in parameters (note that responses are sent in random interval between 0-8 seconds)

private slots:
    void sendResponse();

signals:
    void responded(const QString&);

};

#endif // SAFETYSERVICE_H
