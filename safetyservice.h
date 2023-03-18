#ifndef SAFETYSERVICE_H
#define SAFETYSERVICE_H

#include <QObject>
#include <QTimer>
#include <QRandomGenerator>
#include "elevator.h"

class SafetyService : public QObject
{
    Q_OBJECT
public:
    SafetyService();

public slots:
    void connectWithCar(Elevator* car);

private slots:
    void sendResponse();

signals:
    void responded(const QString&);

};

#endif // SAFETYSERVICE_H
