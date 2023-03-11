#include "mainwindow.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    std::cout << "Hello World!" << std::endl;
    MainWindow w;
    w.show();
    return a.exec();
}
