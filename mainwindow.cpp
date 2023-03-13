#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , elevators(new QVector<Elevator*>)
    , floors(new QVector<Floor*>)
    , ecs(NULL)
    , floorOnUi(NULL)
    , elevatorOnUi(NULL)
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
//    if(ecs != NULL){
//        delete ecs;
//    }

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
    ui->floorRequestComboBox->clear();
    for(int i=0; i<floors->length(); ++i)
        ui->floorRequestComboBox->addItem(QString::number(i));
    ui->floorRequestComboBox->setCurrentIndex(-1);

    setupElevatorInterface();
    ui->elevatorFrame->setVisible(true);
}

void MainWindow::populateFloors(int numFloors) {
    std::cout << "We are populating " << numFloors << " floors" << std::endl;
    for (int i=0; i<numFloors-1; ++i) {
        floors->push_back(new Floor(i, false));
    }
    floors->push_back(new Floor(numFloors-1, true)); // Last floor is the 'top' floor
    for (int i=0; i<numFloors; ++i) {
        std::cout << *(*floors)[i] << std::endl;
    }
    std::cout << "There are " << floors->length() << " floors" << std::endl;

}

void MainWindow::populateElevators(int numCars) {
    std::cout << "We are populating " << numCars << " elevators" << std::endl;
    for (int i=0; i<numCars; ++i) {
        elevators->push_back(new Elevator(i, floors->length()));
    }
    for (int i=0; i<numCars; ++i) {
        std::cout << *(*elevators)[i] << std::endl;
    }
    std::cout << "There are " << elevators->length() << " elevators" << std::endl;
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
    if(!floorOnUi->isTop()) {
        ui->floorUpButton->setVisible(true);
    } else {
        ui->floorUpButton->setVisible(false);
    }
    if(!floorOnUi->getLevel() == Floor::GROUND_LEVEL) {
        ui->floorDownButton->setVisible(true);
    } else {
        ui->floorDownButton->setVisible(false);
    }
}

void MainWindow::on_startSimulationButton_clicked()
{
    //Hide the simulation initialization menu
    ui->initFrame->setVisible(false);

    //Initialize the frames for the simulation
    populateFloors(ui->numFloorsSpinBox->value());
    populateElevators(ui->numElevatorsSpinBox->value());

    //Default floor and elevator shown on the UI are floor number 0 and elevator number 0
    floorOnUi = floors->value(0);
    elevatorOnUi = elevators->value(0);

    initFloorFrame();
    initElevatorFrame();
    ui->simControlFrame->setVisible(true);
}

void MainWindow::on_resetSimulationButton_clicked()
{
//    delete ecs;
//    ecs = NULL;

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
        floorOnUi = floors->value(floorNum);
        setupFloorButtons();
    }
}

void MainWindow::on_floorUpButton_clicked()
{
    floorOnUi->inform(Direction::UP);
}

void MainWindow::on_floorDownButton_clicked()
{
    floorOnUi->inform(Direction::DOWN);
}

void MainWindow::on_carNumComboBox_activated(int carNum)
{
    elevatorOnUi = elevators->value(carNum);
    setupElevatorInterface();
}

void MainWindow::setupElevatorInterface()
{

}

void MainWindow::on_floorRequestComboBox_activated(int floorNum)
{
    if(!elevatorOnUi->isRequestedFloor(floorNum)){ //If not already a floor request...
        elevatorOnUi->destFloorRequest(floorNum);
    }
    ui->floorRequestComboBox->setCurrentIndex(-1);
}
