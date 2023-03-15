#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H

#include <QObject>

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
    void overload();

private:
    int weight;
    const static int MAX_WEIGHT_TOLERANCE;

};

#endif // WEIGHTSENSOR_H
