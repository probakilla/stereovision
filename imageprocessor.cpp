#include "imageprocessor.h"

#include <QImage>

imageprocessor::imageprocessor(QImage image)
{
    _image = image;
    _isCroped = false;
}


void imageprocessor::crop(QRect rect)
{
    _isCroped = true;
    QImage* tmp = new QImage (rect.width(), rect.height(), _image.format());

    int top_y = rect.topLeft().y();
    int top_x = rect.topLeft().x();

    // Copy the left rectangle of the image.
    for (int y = 0; y < rect.height(); ++y)
        for (int x = 0; x < rect.width(); ++x)
            tmp->setPixel(x, y, _image.pixel(x + top_x, y + top_y));

    int top_middle_x = (_image.width() / 2) + top_x;

    // Copy the right rectangle of the image.
    for (int y = 0; y < rect.height(); ++y)
        for (int x = rect.width(); x < tmp->width(); ++x)
            tmp->setPixel(x, y, _image.pixel(x + top_middle_x, y + top_y));

    _image = tmp->copy();
}


void imageprocessor::save()
{
    QFileDialog::getOpenFileName(this, "Enregistrer un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
}


void imageprocessor::affichage()
{
    QWidget* widget = new QWidget;
    widget->setMinimumSize(800, 600);
    _cropedImage = new QLabel(widget);
    _cropedImage->setPixmap(QPixmap::fromImage(_image));
    _cropedImage->show();


    QPushButton *save = new QPushButton("Enregistrer");
    connect(save, SIGNAL(clicked()), this, SLOT(save()));
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
