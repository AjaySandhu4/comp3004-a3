#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , elevators(new QVector<Elevator*>)
    , floors(new QVector<Floor*>)
    , ecs(NULL)
    , floorOnUi(0)
    , elevatorOnUi(0)
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
    initFloorButtons();
    ui->floorFrame->setVisible(true);
}

void MainWindow::initElevatorFrame()
{

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
        elevators->push_back(new Elevator(i));
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

void MainWindow::initFloorButtons() {
    Floor* currFloorOnUi = floors->value(floorOnUi);
    bool isTop = currFloorOnUi->isTop();
    bool isGround = currFloorOnUi->getLevel() == Floor::GROUND_LEVEL;
    if(!isTop) {
        ui->floorUpButton->setVisible(true);
    } else {
        ui->floorUpButton->setVisible(false);
    }
    if(!isGround) {
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
    floorOnUi = 0;
    elevatorOnUi = 0;

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

void MainWindow::on_floorNumComboBox_activated(int index)
{
    floorOnUi = index;
    initFloorButtons();
}

void MainWindow::on_floorUpButton_clicked()
{
    floors->value(floorOnUi)->inform(Floor::UP);
}

void MainWindow::on_floorDownButton_clicked()
{
    floors->value(floorOnUi)->inform(Floor::DOWN);
}
