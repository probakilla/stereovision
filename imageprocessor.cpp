#include "imageprocessor.h"

#include <QImage>

imageprocessor::imageprocessor(const QImage & image) : _image{image}, _processed_image{image}, _is_croped{false} {}


void imageprocessor::crop(const QRect & rect)
{
        _is_croped = true;
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

        _processed_image = tmp->copy();
}

QImage imageprocessor::cvMatToQimage(const cv::Mat &src)
{
    return QImage(src.data, src.cols, src.rows, src.step, _image.format()).copy();
}

cv::Mat imageprocessor::qimageToCvMat(const QImage & image)
{
    return cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
}


QImage imageprocessor::getImage() const
{
    return _image;
}

void imageprocessor::setImage(const QImage & image)
{
    _image = image.copy();
}

bool imageprocessor::getIsCroped() const
{
    return _is_croped;
}

QImage imageprocessor::getProcessedImage() const
{
    return _processed_image;
}

void imageprocessor::imageResize(const int & width,const int & height)
{
    _image = _image.scaled(width, height, Qt::KeepAspectRatio);
}


void imageprocessor::blur()
{
    cv::Mat dest;
    //The strength of the blur is determined by the cv::Size(w, h)
    cv::blur(imageprocessor::qimageToCvMat(getImage()), dest, cv::Size(5, 5));
    cv::imshow("Aperçu du flou", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
}

void imageprocessor::canny()
{
    cv::Mat dest;
    cv::Canny(imageprocessor::qimageToCvMat(getImage()), dest, 100, 200);
    cv::imshow("Aperçu de Canny", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
}

void imageprocessor::sobel()
{
    cv::Mat dest;
    cv::Sobel(imageprocessor::qimageToCvMat(getImage()), dest, -1, 1, 0);
    cv::imshow("Aperçu de Sobel", dest);
    cv::waitKey();
    _processed_image = imageprocessor::cvMatToQimage(dest);
}
