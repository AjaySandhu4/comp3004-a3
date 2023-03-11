#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QDebug>
#include <QString>
#include <iostream>

#include "ecs.h"
#include "elevator.h"
#include "floor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ECS *ecs;
    QVector<Floor*> *floors;
    QVector<Elevator*> *elevators;



    void initFloorFrame();
    void initElevatorFrame();
    void populateFloors(int numFloors);
    void populateElevators(int numCars);
    void clearFloors();
    void clearElevators();
    void initFloorButtons(int floorNum);

private slots:
    void initSimulation();
    void resetSimulation();
};
#endif // MAINWINDOW_H
