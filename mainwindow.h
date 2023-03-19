#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QDebug>
#include <QString>
#include <QTextStream>

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

    ECS *ecs; //The control system of the simulation
    QVector<Floor*> *floors; //The floors of the simulation
    QVector<Elevator*> *elevators; //The elevators of the simulation

    Floor *floorOnUi; //The floor currently displayed on the floor UI for interaction
    Elevator *elevatorOnUi; //The elevator currently displayed on the floor UI for interaction
    AllocationStrategy *currAllocationStrategy; //The allocation strategy that the simulation was started with

    const static int SIM_PASSENGER_WEIGHT; //The weight of a simulated passenger (used for when they embark or disembark an elevator)

    void initFloorFrame(); //Initialize the frame housing the floor UI
    void initElevatorFrame(); //Initialize the frame housing the elevator UI
    void populateFloors(int numFloors); //Populate the floors QVector with the specified number
    void populateElevators(int numCars); //Populate the elevators QVector with the specified number
    void clearFloors(); //Empty the floors QVector once simulation is ended
    void clearElevators(); //Empty the elevators QVector once simulation is ended
    void setupElevatorInterface(); //Setup interactive elevator UI
    void disconnectElevatorFromUi(); //Disconnect signals and slots from the elevator in the UI
    void initECS();

private slots:
    void setupFloorButtons(); //Render the UP and/or DOWN buttons on floor UI
    void toggleWeightButtons(); //Render the Embark and/or Disembark buttons on elevator UI
    void setTextDisplay(const QString&); //Set the Text Display text browser
    void setAudioOutput(const QString&); //Set the Audio Output text browser

    void on_startSimulationButton_clicked(); //Start the simulation
    void on_resetSimulationButton_clicked(); //Reset the simulation
    void on_floorNumComboBox_activated(int floorNum); //Change the floor in the UI
    void on_floorUpButton_clicked();
    void on_floorDownButton_clicked();
    void on_carNumComboBox_activated(int carNum); //Change the elevator in the UI
    void on_elevatorFloorRequestComboBox_activated(int floorNum); //Make a destination floor request for the elevator in the UI

    void on_helpButton_clicked(); //Make the elevator in the UI signal for help
    void on_embarkButton_clicked(); //Add weight onto weight sensor and/or trigger door light sensor of elevator on UI
    void on_disembarkButton_clicked(); //Reduce weight onto weight sensor and/or trigger door light sensor of elevator on UI
    void on_elevatorFireAlarmButton_clicked(); //Make elevator in the UI handle fire
    void on_openDoorButton_clicked(); //Make elevator in the UI keep its doors open for longer
    void on_closeDoorButton_released(); //Make elevator in the UI close its doors sooner
    void on_speakButton_clicked(); //Inform elevator that the pressing of the help button was followed up with by a response by the caller of help
    void on_buildingFireButton_clicked(); //Inform the ECS of fire in the building
    void on_powerOutageButton_clicked(); //Inform the ECS of a power outage in the building
};
#endif // MAINWINDOW_H
