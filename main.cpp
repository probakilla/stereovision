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
    std::string right_im = "julpilleux/img/6cm/right_0.png";
    std::string left_im = "julpilleux/img/6cm/left_0.png";

    cv::Mat right = cv::imread(right_im, cv::IMREAD_COLOR);
    if ( right.empty() )
    {
        std::cout<<"Cannot read image file: "<< right_im;
        return -1;
    }

    cv::Mat left = cv::imread(left_im, cv::IMREAD_COLOR);
    if ( left.empty() )
    {
        std::cout<<"Cannot read image file: "<< left_im;
        return -1;
    }

    rc.disparity( left, right );

    return 0;
}
