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

cv::Mat imageprocessor::qimageToCvMat(const QImage &image)
{
    return cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
}


QImage imageprocessor::getImage() const
{
    return _image;
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

/*  A EFFACER UNE FOIS LU
 *  Je ne modifie rien pour l'instant mais il faudrait que tu mette le résultat de cette fonction dans la donnée membre _processed_image
 *  (donc ta méthode devient void) pour faciliter l'exercie 3 (afficher l'image plus celle floutée à coté)
 *  Et pour ta fonction je ne pense pas que tu ai besoin de paramètres, prend direct la donnée membre _image sans la modifiée.
 */
/*QImage imageprocessor::blur(QImage img)
{
    cv::Mat imgMat;
    cv::Mat imgDst;
    //imgMat = qImageToCvMat();
    cv::blur(imgMat, imgDst, 1);
    cv::imshow("Aperçu du flou", imgDst);
    QImage ret = cvMatToQImage(&imgDst, ?)
}*/
