#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QPixmap>
#include <QRect>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QSize>
#include <QString>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "opencv2/contrib/contrib.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

class imageprocessor: public QWidget
{
    Q_OBJECT
public:
    imageprocessor(const QImage & getLeftImage);
    void imageResize(const int & width,const int & height);
    void crop(const QRect & rect);
    QImage cvMatToQimage(const cv::Mat & src, bool canny = false);
    QImage getLeftImage() const;
    QImage getRightImage() const;
    void setImage(const QImage & image);
    void setImageAlt(const QImage & image);

    cv::Mat qimageToCvMat (const QImage & getLeftImage);
    bool getIsCroped() const;
    void blur();
    void canny();
    void sobel();
    void calibrateCameras();
    void disparityMap ();
    void featureDetection();
    void featureMatching();
    void splitImage ();
    void showKeyPoints();
    QImage getProcessedImage () const;

private:
    QImage _left_image;
    QImage _right_image;// The right part of the image
    QImage _processed_image;
    std::vector<cv::KeyPoint> left_keypoints, right_keypoints;
    bool _is_croped;

    /// Constantes symboliques du Blur.
    const int BLUR_X_SIZE = 5;
    const int BLUR_Y_SIZE = 5;

    /// Constantes symboliques du Canny.
    const int THRESHOLD_1 = 100;
    const int THRESHOLD_2 = 200;

    /// Constantes symboliques de calibrateCameras.
    const int CALIBRATE_X_SIZE = 9;
    const int CALIBRATE_Y_SIZE = 14;
    const int CORNER_WIN_X = 5;
    const int CORNER_WIN_Y = 5;
    const int CORNER_ZERO_X = -1;
    const int CORNER_ZERO_Y = -1;
    const int CALI_MAX_COUNT = 30;
    const double CALI_EPSILON = 0.1;

    /// Constantes symboliques du SGBM.
    const int SAD_SIZE = 5;
    const int NB_DISPARITIES = 192;
    const int PRE_FILTER_CAP = 4;
    const int MIN_DISP = -52;
    const int UNI_RATIO = 5;
    const int SPECKLE_WIN_SIZE = 150;
    const int SPECKLE_RANGE = 2;
    const int MAX_DIFF = 10;
    const int DISP_SMOOTHNESS_1 = SAD_SIZE*SAD_SIZE*8*3;
    const int DISP_SMOOTHNESS_2 = SAD_SIZE*SAD_SIZE*32*3;
    const int RANGE_INF = 0;
    const int RANGE_SUP = 255;

    /// Constante symbolique de featureDetection.
    const int MIN_HESSIAN = 400;

    /// Constantes symboliques de featureMatching.
    const int MAX_DIST = 0;
    const int MIN_DIST = 100;
};

#endif // IMAGEPROCESSOR_H
