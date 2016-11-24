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

class imageprocessor: public QWidget
{
    Q_OBJECT
public:
    imageprocessor(const QImage & getImage);
    void imageResize(const int & width,const int & height);
    void crop(const QRect & rect);
    QImage cvMatToQimage(const cv::Mat & src);
    QImage getImage() const;
    void setImage(const QImage & image);
    void splitImage ();

    cv::Mat qimageToCvMat (const QImage & getImage);
    bool getIsCroped() const;
    void blur();
    void canny();
    void sobel();
    void disparity_map ();

    QImage getProcessedImage () const;

private:
    QImage _image;
    QImage _image_alt;// The right part of the image
    QImage _processed_image;
    bool _is_croped;
    void divide_image ();
};

#endif // IMAGEPROCESSOR_H
