#include "mainwindow.h"
#include "robot_controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();*/

    robot_controller rc;
    rc.test();

    return 0;
}
