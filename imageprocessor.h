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
    imageprocessor(const QImage & getImage);
    void imageResize(const int & width,const int & height);
    void crop(const QRect & rect);
    QImage cvMatToQimage(const cv::Mat & src);
    QImage getImage() const;
    void setImage(const QImage & image);

    cv::Mat qimageToCvMat (const QImage & getImage);
    bool getIsCroped() const;
    void blur();
    void canny();
    void sobel();

    QImage getProcessedImage () const;

private:
    QImage _image;
    QImage _processed_image;
    bool _is_croped;
};

#endif // IMAGEPROCESSOR_H
