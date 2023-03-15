#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , elevators(new QVector<Elevator*>)
    , floors(new QVector<Floor*>)
    , ecs(nullptr)
    , floorOnUi(nullptr)
    , elevatorOnUi(nullptr)
    , currAllocationStrategy(nullptr)
{
    ui->setupUi(this);

    //Hide simulation frames before initialization
    ui->elevatorFrame->setVisible(false);
    ui->floorFrame->setVisible(false);
    ui->simControlFrame->setVisible(false);

}

MainWindow::~MainWindow()
{
    std::cout << "Main window destructor called" << std::endl;
    if(ecs != nullptr){
        delete ecs;
    }

    if(currAllocationStrategy != nullptr){
        delete currAllocationStrategy;
    }

    clearElevators();
    delete elevators;

    clearFloors();
    delete floors;

    delete ui;
}

void MainWindow::initFloorFrame()
{
    ui->floorNumComboBox->clear();
    for(int i=0; i<floors->length(); ++i)
        ui->floorNumComboBox->addItem(QString::number(i));
    setupFloorButtons();
    ui->floorFrame->setVisible(true);
}

void MainWindow::initElevatorFrame()
{
    //Initialize combobox with car numbers
    ui->carNumComboBox->clear();
    for(int i=0; i<elevators->length(); ++i)
        ui->carNumComboBox->addItem(QString::number(i));

    //Initalize combobox with floor numbers (to make requests)
    ui->elevatorFloorRequestComboBox->clear();
    for(int i=0; i<floors->length(); ++i)
        ui->elevatorFloorRequestComboBox->addItem(QString::number(i));
    ui->elevatorFloorRequestComboBox->setCurrentIndex(-1);

    setupElevatorInterface();
    ui->elevatorFrame->setVisible(true);
}

void MainWindow::populateFloors(int numFloors) {
    for (int i=0; i<numFloors-1; ++i) {
        floors->push_back(new Floor(i, false));
    }
    floors->push_back(new Floor(numFloors-1, true)); // Last floor is the 'top' floor
}

void MainWindow::populateElevators(int numCars) {
    for (int i=0; i<numCars; ++i) {
        elevators->push_back(new Elevator(i, floors->length()));
    }
}

void MainWindow::clearFloors() {
    for (int i=0; i<floors->length(); ++i)
        delete floors->value(i);
    floors->clear();
}

void MainWindow::clearElevators() {
    for (int i=0; i<elevators->length(); ++i)
        delete elevators->value(i);
    elevators->clear();
}

void MainWindow::setupFloorButtons() {
    if(floorOnUi->isTop()) ui->floorUpButton->setVisible(false);
    else ui->floorUpButton->setVisible(true);

    if(floorOnUi->getLevel() == Floor::GROUND_LEVEL) ui->floorDownButton->setVisible(false);
    else ui->floorDownButton->setVisible(true);

    if(floorOnUi->isWaitingUp()) ui->floorUpButton->setStyleSheet("background-color: yellow");
    else ui->floorUpButton->setStyleSheet("");

    if(floorOnUi->isWaitingDown()) ui->floorDownButton->setStyleSheet("background-color: yellow");
    else ui->floorDownButton->setStyleSheet("");
}

void MainWindow::on_startSimulationButton_clicked()
{
    //Hide the simulation initialization menu
    ui->initFrame->setVisible(false);

    //Initialize the frames for the simulation
    populateFloors(ui->numFloorsSpinBox->value());
    populateElevators(ui->numElevatorsSpinBox->value());

    initECS();

    //Default floor and elevator shown on the UI are floor number 0 and elevator number 0
    floorOnUi = floors->value(0);
    connect(floorOnUi, &Floor::floorServiced, this, &MainWindow::setupFloorButtons);
    elevatorOnUi = elevators->value(0);

    initFloorFrame();
    initElevatorFrame();
    ui->simControlFrame->setVisible(true);
}

void MainWindow::on_resetSimulationButton_clicked()
{
    delete ecs;
    ecs = nullptr;

    delete currAllocationStrategy;
    currAllocationStrategy = nullptr;

    clearFloors();
    clearElevators();

    //Hide simulation frames
    ui->elevatorFrame->setVisible(false);
    ui->floorFrame->setVisible(false);
    ui->simControlFrame->setVisible(false);

    //Show the simulation initialization menu again
    ui->initFrame->setVisible(true);
}

void MainWindow::on_floorNumComboBox_activated(int floorNum)
{
    if(floorNum != floorOnUi->getLevel()){
        //Disconnect current floor on the UI from the UI floor buttons
        disconnect(floorOnUi, nullptr, ui->floorUpButton, nullptr);
        disconnect(floorOnUi, nullptr, ui->floorDownButton, nullptr);

        //Switch floor on the UI to the newly selected floor
        floorOnUi = floors->value(floorNum);
        connect(floorOnUi, &Floor::floorServiced, this, &MainWindow::setupFloorButtons);

        //Setup floor buttons to new floor
        setupFloorButtons();
    }
}

void MainWindow::on_floorUpButton_clicked()
{
    ui->floorUpButton->setStyleSheet("background-color: yellow");
    floorOnUi->inform(Direction::UP);
}

void MainWindow::on_floorDownButton_clicked()
{
    ui->floorDownButton->setStyleSheet("background-color: yellow");
    floorOnUi->inform(Direction::DOWN);
}

void MainWindow::on_carNumComboBox_activated(int carNum)
{
    if(carNum != elevatorOnUi->getCarNum()){
        disconnect(elevatorOnUi, nullptr, ui->currFloorDisplay, nullptr);
        elevatorOnUi = elevators->value(carNum);
        setupElevatorInterface();
    }
}

void MainWindow::setupElevatorInterface()
{
    //Setup current floor display
    connect(elevatorOnUi, SIGNAL(reachedFloor(int)), ui->currFloorDisplay, SLOT(display(int)));
    ui->currFloorDisplay->display(elevatorOnUi->getCurrFloor());
}

void MainWindow::on_elevatorFloorRequestComboBox_activated(int floorNum)
{
    if(!elevatorOnUi->isRequestedFloor(floorNum)){ //If not already a floor request...
        elevatorOnUi->destFloorRequest(floorNum);
    }
    ui->elevatorFloorRequestComboBox->setCurrentIndex(-1);
}

//Disconnect elements of UI that are connected to current elevator on UI
void MainWindow::disconnectElevatorFromUi() {
    disconnect(elevatorOnUi, nullptr, ui->currFloorDisplay, nullptr);
}

void MainWindow::initECS() {
    bool isTimeDependentStrategy = ui->timeDependencyCheckBox->isChecked();
    currAllocationStrategy = isTimeDependentStrategy ? static_cast<AllocationStrategy*>(new TimeDependentStrategy()) : static_cast<AllocationStrategy*>(new ElevatorCenteredStrategy());
    ecs = new ECS(currAllocationStrategy, floors, elevators);
}
