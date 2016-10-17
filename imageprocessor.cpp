#include "imageprocessor.h"

#include <QImage>

imageprocessor::imageprocessor(QImage image)
{
    _image = image;
    _is_cropped = false;
}


void imageprocessor::crop(const QRect & rect)
{
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

QImage imageprocessor::image() const
{
    return _image;
}

QPushButton *imageprocessor::getB_valider() const
{
    return _b_valider;
}

void imageprocessor::validate()
{
    _is_cropped = true;
}

bool imageprocessor::getIs_cropped() const
{
    return _is_cropped;
}

void imageprocessor::display()
{
    QWidget* widget = new QWidget;
    widget->setMinimumSize(800, 600);
    widget->setAccessibleDescription("Voulez sauvegarder cette image");
    QLabel *_cropedImage = new QLabel(widget);
    _cropedImage->setPixmap(QPixmap::fromImage(_image));
    _cropedImage->show();


    _b_valider = new QPushButton("Valider");
    connect(_b_valider, SIGNAL(clicked()), this, SLOT(validate()));
    connect(_b_valider, SIGNAL(clicked()), this, SLOT(close()));
    QPushButton *b_cancel = new QPushButton("Annuler");
    connect(b_cancel, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(_b_valider);
    buttonLayout->addWidget(b_cancel);
    QVBoxLayout *primeLayout = new QVBoxLayout;
    primeLayout->addWidget(widget);
    primeLayout->addLayout(buttonLayout);

    this->setLayout(primeLayout);
    this->show();
}

void imageprocessor::imageResize(const int & width,const int & height)
{
    _image = _image.scaled(width, height, Qt::KeepAspectRatio);
}
