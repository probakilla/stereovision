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
    void display();
    void imageResize(const int & width,const int & height);
    void crop(const QRect & rect);


    QImage image() const;

    bool is_valid() const;

public slots:
    void validate();



private:
    QImage _image;
    bool _is_valid;
};

#endif // IMAGEPROCESSOR_H
