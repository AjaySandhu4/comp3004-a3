#ifndef DOORLIGHTSENSOR_H
#define DOORLIGHTSENSOR_H

#include <QObject>

// Purpose of class: Used to interrupt Elevator during its closing sequence
class DoorLightSensor : public QObject
{
    Q_OBJECT
public:
    DoorLightSensor();
    void setActive(bool active);
    void detects();

signals:
    void triggered();

private:
    bool active; //If active, the triggered() signal can be sent...

};

#endif // DOORLIGHTSENSOR_H
