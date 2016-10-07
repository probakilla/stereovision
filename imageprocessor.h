#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QPixmap>
#include <QRect>

class imageprocessor
{

public:
    imageprocessor(QPixmap image);
    void affichage();
    void crop(QRect rect);

private:
    QPixmap _image;
    bool _isCroped;
};

#endif // IMAGEPROCESSOR_H
