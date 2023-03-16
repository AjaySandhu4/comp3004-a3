#ifndef DOORLIGHTSENSOR_H
#define DOORLIGHTSENSOR_H

#include <QObject>

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
    bool active;

};

#endif // DOORLIGHTSENSOR_H
