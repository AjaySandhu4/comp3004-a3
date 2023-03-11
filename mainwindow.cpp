#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Hide simulation frames before initialization
    ui->elevatorFrame->setVisible(false);
    ui->floorFrame->setVisible(false);
    ui->simControlFrame->setVisible(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}

