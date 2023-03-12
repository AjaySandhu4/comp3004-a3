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

    int floorOnUi;
    int elevatorOnUi;

    void initFloorFrame();
    void initElevatorFrame();
    void populateFloors(int numFloors);
    void populateElevators(int numCars);
    void clearFloors();
    void clearElevators();
    void initFloorButtons();

private slots:
    void on_startSimulationButton_clicked();
    void on_resetSimulationButton_clicked();
    void on_floorNumComboBox_activated(int index);
    void on_floorUpButton_clicked();
    void on_floorDownButton_clicked();
};
#endif // MAINWINDOW_H
