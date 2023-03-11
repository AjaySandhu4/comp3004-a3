#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , elevators(new QVector<Elevator*>)
    , floors(new QVector<Floor*>)
    , ecs(NULL)
{
    ui->setupUi(this);

    //Hide simulation frames before initialization
    ui->elevatorFrame->setVisible(false);
    ui->floorFrame->setVisible(false);
    ui->simControlFrame->setVisible(false);

    //Connect start and reset simulation buttons
    connect(ui->startSimulationButton, &QPushButton::released, this, &MainWindow::initSimulation);
    connect(ui->resetSimulationButton, &QPushButton::released, this, &MainWindow::resetSimulation);

}

MainWindow::~MainWindow()
{
    std::cout << "Main window destructor called" << std::endl;
    if(ecs != NULL){
        delete ecs;
    }

    clearElevators();
    delete elevators;

    clearFloors();
    delete floors;

    delete ui;
}

void MainWindow::initSimulation()
{
    //Hide the simulation initialization menu
    ui->initFrame->setVisible(false);

    //Initialize the frames for the simulation
//    ui->elevatorFrame->setVisible(true);
//    ui->floorFrame->setVisible(true);
    populateFloors(ui->numFloorsSpinBox->value());
    populateElevators(ui->numElevatorsSpinBox->value());

    initFloorFrame();
    initElevatorFrame();
    ui->simControlFrame->setVisible(true);
}

void MainWindow::resetSimulation()
{
    delete ecs;
    ecs = NULL;

    clearFloors();
    clearElevators();

    //Hide simulation frames
    ui->elevatorFrame->setVisible(false);
    ui->floorFrame->setVisible(false);
    ui->simControlFrame->setVisible(false);

    //Show the simulation initialization menu again
    ui->initFrame->setVisible(true);
}

void MainWindow::initFloorFrame()
{

}

void MainWindow::initElevatorFrame()
{

}

void MainWindow::populateFloors(int numFloors) {
    std::cout << "We are populating " << numFloors << " floors" << std::endl;
    for (int i=1; i<numFloors; ++i) {
        floors->push_back(new Floor(i, false));
    }
    floors->push_back(new Floor(numFloors, true)); // Last floor is the 'top' floor
    for (int i=0; i<numFloors; ++i) {
        std::cout << *(*floors)[i] << std::endl;
    }
    std::cout << "There are " << floors->length() << " floors" << std::endl;

}

void MainWindow::populateElevators(int numCars) {
    std::cout << "We are populating " << numCars << " elevators" << std::endl;
    for (int i=1; i<=numCars; ++i) {
        elevators->push_back(new Elevator(i));
    }
    for (int i=0; i<numCars; ++i) {
        std::cout << *(*elevators)[i] << std::endl;
    }
    std::cout << "There are " << elevators->length() << " elevators" << std::endl;
}

void MainWindow::clearFloors() {
    for (int i=1; i<=floors->length(); ++i)
        delete floors->value(i);
    floors->clear();
}

void MainWindow::clearElevators() {
    for (int i=1; i<=elevators->length(); ++i)
        delete elevators->value(i);
    elevators->clear();
}
