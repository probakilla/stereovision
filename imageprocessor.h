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


    QPushButton *getB_valider() const;

    bool getIs_cropped() const;

public slots:
    void validate();


private:
    QImage _image;
    QPushButton *_b_valider;
    bool _is_cropped;
};

#endif // IMAGEPROCESSOR_H
