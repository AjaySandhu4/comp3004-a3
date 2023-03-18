#include "mainwindow.h"
#include "ui_mainwindow.h"

const int MainWindow::SIM_PASSENGER_WEIGHT = 80;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ecs(nullptr)
    , floors(new QVector<Floor*>)
    , elevators(new QVector<Elevator*>)
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

    ui->elevatorFrame->setVisible(true);
    setupElevatorInterface();
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
    QTextStream(stdout) << "User has pressed the " << Direction::UP << " button from floor " << floorOnUi->getLevel() << "..." << endl;
}

void MainWindow::on_floorDownButton_clicked()
{
    ui->floorDownButton->setStyleSheet("background-color: yellow");
    floorOnUi->inform(Direction::DOWN);
    QTextStream(stdout) << "User has pressed the " << Direction::DOWN << " button from floor " << floorOnUi->getLevel() << "..." << endl;
}

void MainWindow::on_carNumComboBox_activated(int carNum)
{
    if(carNum != elevatorOnUi->getCarNum()){
        disconnectElevatorFromUi();
        elevatorOnUi = elevators->value(carNum);
        setupElevatorInterface();
    }
}

void MainWindow::setupElevatorInterface()
{
    //Setup current floor display
    connect(elevatorOnUi, SIGNAL(reachedFloor(int)), ui->currFloorDisplay, SLOT(display(int)));
    ui->currFloorDisplay->display(elevatorOnUi->getCurrFloor());

    connect(elevatorOnUi, SIGNAL(voice(const QString&)), this, SLOT(setAudioOutput(const QString&)));
    connect(elevatorOnUi, SIGNAL(textMessage(const QString&)), this, SLOT(setTextDisplay(const QString&)));

    connect(elevatorOnUi, &Elevator::floorServiced, this, &MainWindow::setupElevatorInterface);

    connect(elevatorOnUi, &Elevator::moving, this, &MainWindow::toggleWeightButtons);
    toggleWeightButtons();
}

void MainWindow::toggleWeightButtons() {
    // Weight buttons are only shown if the doors are open
    ui->embarkButton->setVisible(elevatorOnUi->areDoorsOpen());
    ui->disembarkButton->setVisible(elevatorOnUi->areDoorsOpen() && !elevatorOnUi->isEmpty()); //Only show disembark button if elevator is also not empty
}

void MainWindow::on_elevatorFloorRequestComboBox_activated(int floorNum)
{
    if(!elevatorOnUi->isRequestedFloor(floorNum)){ //If not already a floor request...
        elevatorOnUi->destFloorRequest(floorNum);
    }
    ui->elevatorFloorRequestComboBox->setCurrentIndex(-1);
    QTextStream(stdout) << "User has made a request for floor " << floorNum << " from elevator " << elevatorOnUi->getCarNum() << "..." << endl;
}

//Disconnect elements of UI that are connected to current elevator on UI
void MainWindow::disconnectElevatorFromUi() {
    disconnect(elevatorOnUi, nullptr, ui->currFloorDisplay, nullptr);
    disconnect(elevatorOnUi, nullptr, this, nullptr);
}

void MainWindow::initECS() {
    bool isTimeDependentStrategy = ui->timeDependencyCheckBox->isChecked();
    currAllocationStrategy = isTimeDependentStrategy ? static_cast<AllocationStrategy*>(new TimeDependentStrategy()) : static_cast<AllocationStrategy*>(new ElevatorCenteredStrategy());
    ecs = new ECS(currAllocationStrategy, floors, elevators);
}

void MainWindow::on_helpButton_clicked()
{
//    elevatorOnUi->handleHelp();
    ui->speakButton->setVisible(true);
}

void MainWindow::on_embarkButton_clicked()
{
    QTextStream(stdout) << "User has embarked elevator " << elevatorOnUi->getCarNum() << "..." << endl;
    elevatorOnUi->getWeightSensor()->addWeight(SIM_PASSENGER_WEIGHT);
    toggleWeightButtons();
    elevatorOnUi->getDoorLightSensor()->detects();
}

void MainWindow::on_disembarkButton_clicked()
{
    QTextStream(stdout) << "User has disembarked elevator " << elevatorOnUi->getCarNum() << "..." << endl;
    elevatorOnUi->getWeightSensor()->reduceWeight(SIM_PASSENGER_WEIGHT); 
    toggleWeightButtons();
    elevatorOnUi->getDoorLightSensor()->detects();
}

void MainWindow::on_elevatorFireAlarmButton_clicked()
{
    elevatorOnUi->handleFire();
    QTextStream(stdout) << "User has pulled fire alarm in elevator " << elevatorOnUi->getCarNum() << "!" << endl;
}

void MainWindow::on_openDoorButton_clicked()
{
    elevatorOnUi->onOpenButtonPress();
}

void MainWindow::on_closeDoorButton_released()
{
    elevatorOnUi->onCloseButtonRelease();
}

void MainWindow::on_speakButton_clicked()
{
//    elevatorOnUi->receiveMicInput();
}

void MainWindow::on_buildingFireButton_clicked()
{
    ecs->handleFire();
}

void MainWindow::on_powerOutageButton_clicked()
{
    ecs->handlePowerOut();
}

void MainWindow::setTextDisplay(const QString& msg)
{
    ui->textDisplay->setText(msg);
    QTimer::singleShot(5000, ui->textDisplay, &QTextBrowser::clear);
}

void MainWindow::setAudioOutput(const QString& msg)
{
    ui->audioOutput->setText(msg);
    QTimer::singleShot(5000, ui->audioOutput, &QTextBrowser::clear);
}


