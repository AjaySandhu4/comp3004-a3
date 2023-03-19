#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H

#include <QObject>

// Purpose of class: Used to keep track of weight of Elevator contents and may send overload signal
class WeightSensor : public QObject
{
    Q_OBJECT
public:
    WeightSensor();

    int overloadedBy() const;
    bool isLoaded() const;

public slots:
    void addWeight(int weightAdded);
    void reduceWeight(int weightReduced);

signals:
    void overload(); //Signal that is sent to elevator in case weight tolerance is exceeded

private:
    int weight;
    const static int MAX_WEIGHT_TOLERANCE; //The max weight the elevator can tolerate

};

#endif // WEIGHTSENSOR_H
