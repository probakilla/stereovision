#include "imageprocessor.h"

#include <QImage>

imageprocessor::imageprocessor(QImage image)
{
    _image = image;
    _isCroped = false;
}


void imageprocessor::crop(QRect rect)
{
    if(_isCroped == false)
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
        this->affichage();
    }
}

/*
void imageprocessor::crop(QRect rect)
{
    if(_isCroped == false)
    {

        QImage* tmp = new QImage ((rect.width() * 2), rect.height(), _image.format());

        for (int y = rect.)
    }
*/

void imageprocessor::save()
{
    QString path = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    _isCroped = true;
    _image.save(path);
}


void imageprocessor::affichage()
{
    QWidget* widget = new QWidget;
    widget->setMinimumSize(800, 600);
    widget->setAccessibleDescription("Voulez sauvegarder cette image");
    QLabel *_cropedImage = new QLabel(widget);
    _cropedImage->setPixmap(QPixmap::fromImage(_image));
    _cropedImage->show();


    QPushButton *save = new QPushButton("Enregistrer");
    connect(save, SIGNAL(clicked()), this, SLOT(save()));
    connect(save, SIGNAL(clicked()), this, SLOT(close()));
    QPushButton *cancel = new QPushButton("Annuler");
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(save);
    buttonLayout->addWidget(cancel);
    QVBoxLayout *primeLayout = new QVBoxLayout;
    primeLayout->addWidget(widget);
    primeLayout->addLayout(buttonLayout);

    this->setLayout(primeLayout);
    this->show();
}

void imageprocessor::imageResize(int width, int height)
{
    _image = _image.scaled(width, height, Qt::KeepAspectRatio);
}
