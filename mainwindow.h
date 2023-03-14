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
#include "direction.h"
#include "elevatorcenteredstrategy.h"
#include "timedependentstrategy.h"

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

    Floor *floorOnUi;
    Elevator *elevatorOnUi;
    AllocationStrategy *currAllocationStrategy;

    void initFloorFrame();
    void initElevatorFrame();
    void populateFloors(int numFloors);
    void populateElevators(int numCars);
    void clearFloors();
    void clearElevators();
    void setupElevatorInterface();
    void disconnectElevatorFromUi();
    void initECS();

private slots:
    void on_startSimulationButton_clicked();
    void on_resetSimulationButton_clicked();
    void on_floorNumComboBox_activated(int floorNum);
    void on_floorUpButton_clicked();
    void on_floorDownButton_clicked();
    void on_carNumComboBox_activated(int carNum);
    void on_floorRequestComboBox_activated(int floorNum);
    void setupFloorButtons();
};
#endif // MAINWINDOW_H
