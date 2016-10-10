#include "imageprocessor.h"

imageprocessor::imageprocessor(QPixmap image)
{
    _image = image;
    _isCroped = false;
}


void imageprocessor::crop(QRect rect)
{

}


void imageprocessor::save()
{
    QFileDialog::getOpenFileName(this, "Enregistrer un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
}


void imageprocessor::affichage()
{
    QWidget* widget = new QWidget;
    //_cropedImage = new QLabel(widget);
    //_cropedImage->show();

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
