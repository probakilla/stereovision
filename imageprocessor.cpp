#include "imageprocessor.h"

#include <QImage>

imageprocessor::imageprocessor(QImage image)
{
    _image = image;
    _is_cropped = false;
}


void imageprocessor::crop(const QRect & rect)
{
        _is_cropped = true;
        QImage* tmp = new QImage ((rect.width() * 2), rect.height(), _image.format());

        int top_y = rect.topLeft().y();
        int top_x = rect.topLeft().x();

        // Copy the left rectangle of the image.
        for (int y = 0; y < rect.height(); ++y)
            for (int x = 0; x < rect.width(); ++x)
                tmp->setPixel(x, y, _image.pixel(x + top_x, y + top_y));

        int top_middle_x = (_image.width() / 2) + top_x;

        // Copy the right rectangle of the image.
        for (int y = 0; y < rect.height(); ++y)
            for (int x = 0; x < (tmp->width() / 2); ++x)
                tmp->setPixel(x + (tmp->width() / 2), y, _image.pixel(x + top_middle_x, y + top_y));

        _image = tmp->copy();
}

QImage imageprocessor::cvMatToQImage(const cv::Mat &src, QImage::Format format)
{
    return QImage(src.data, src.cols, src.rows, src.step, format).copy();
}

QImage imageprocessor::image() const
{
    return _image;
}

void imageprocessor::validate()
{
    _is_cropped = true;
}

bool imageprocessor::getIs_cropped() const
{
    return _is_cropped;
}

void imageprocessor::imageResize(const int & width,const int & height)
{
    _image = _image.scaled(width, height, Qt::KeepAspectRatio);
}
