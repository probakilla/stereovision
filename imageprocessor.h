#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QPixmap>
#include <QRect>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>

class imageprocessor: public QWidget
{
    Q_OBJECT
public:
    imageprocessor(QPixmap image);
    void affichage();
    void crop(QRect rect);


public slots:
    void save();


private:
    QPixmap _image;
    QLabel *_cropedImage;
    bool _isCroped;
};

#endif // IMAGEPROCESSOR_H
