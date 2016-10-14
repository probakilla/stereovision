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



class imageprocessor: public QWidget
{
    Q_OBJECT
public:
    imageprocessor(QImage image);
    void affichage();
    void imageResize(int width, int height);
    void crop(QRect rect);


public slots:
    void save();



private:
    QImage _image;
    bool _isCroped;
};

#endif // IMAGEPROCESSOR_H
