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
    std::string right_im = "julpilleux/img/damier_10cm/right_10.png";
    std::string left_im = "julpilleux/img//damier_10cm//left_10.png";
    std::string right_im2 = "julpilleux/img/damier_10cm/right_11.png";
    std::string left_im2 = "julpilleux/img/damier_10cm/left_11.png";

    cv::Mat right = cv::imread(right_im, cv::IMREAD_COLOR);
    if ( right.empty() ){
        std::cout<<"Cannot read image file: "<< right_im;
        return -1;
    }
    cv::Mat left = cv::imread(left_im, cv::IMREAD_COLOR);
    if ( left.empty() ){
        std::cout<<"Cannot read image file: "<< left_im;
        return -1;
    }

    cv::Mat right2 = cv::imread(right_im2, cv::IMREAD_COLOR);
    if ( right2.empty() ){
        std::cout<<"Cannot read image file: "<< right_im2;
        return -1;
    }
    cv::Mat left2 = cv::imread(left_im2, cv::IMREAD_COLOR);
    if ( left2.empty() ){
        std::cout<<"Cannot read image file: "<< left_im2;
        return -1;
    }

    auto disp = rc.disparity( left, right );
    auto disp2 = rc.disparity( left2, right2 );
    auto diff = rc.diff( disp, disp2);

    cv::imshow("base", right);
    cv::imshow("base2", right2);
    cv::imshow("disp", disp);
    cv::imshow("disp2", disp2);
    cv::imshow("img", diff);

    cv::waitKey();

    return 0;
}
