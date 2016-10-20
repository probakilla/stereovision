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

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class imageprocessor: public QWidget
{
    Q_OBJECT
public:
    imageprocessor(const QImage & image);
    void imageResize(const int & width,const int & height);
    void crop(const QRect & rect);
    QImage cvMatToQimage(const cv::Mat & src);
    QImage image() const;

    cv::Mat qimageToCvMat (const QImage & image);
    bool getIsCroped() const;
    //QImage imageprocessor::blur(QImage img);

    QImage getImageBlurred() const;

public slots:
    void validate();


private:
    QImage _image;
    QImage _image_blurred;
    bool _is_cropped;
};

#endif // IMAGEPROCESSOR_H
