#include "imageprocessor.h"

#include <QImage>

ImageProcessor::ImageProcessor(QImage image) : QWidget(), _m_image (image), _m_is_croped (false) {}


void ImageProcessor::crop(QRect const & rect)
{
    if (!_m_is_croped)
    {
        // The _m_image will not be 'croptable' anymore.
        _m_is_croped = true;

        // Create a temporary image that will be in _m_image. It width will be the double of rect's width.
        QImage* tmp = new QImage (rect.width() * 2, rect.height(), _m_image.format());

        int top_y = rect.topLeft().y();
        int top_x = rect.topLeft().x();

        // Copy the left rectangle of the image.
        for (int y = 0; y < rect.height(); ++y)
            for (int x = 0; x < rect.width(); ++x)
                tmp->setPixel(x, y, _m_image.pixel(x + top_x, y + top_y));

        int top_middle_x = (_m_image.width() / 2) + top_x;

        // Copy the right rectangle of the image.
        for (int y = 0; y < rect.height(); ++y)
            for (int x = rect.width(); x < tmp->width(); ++x)
                tmp->setPixel(x, y, _m_image.pixel(x + top_middle_x, y + top_y));

        // Replace the old _m_image.
        _m_image = tmp->copy();
    }
}


void ImageProcessor::save()
{
    QString path = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    _m_image.save(path);
}


void ImageProcessor::affichage()
{
    QWidget* widget = new QWidget;
    widget->setMinimumSize(800, 600);
    QLabel* cropedImage = new QLabel(widget);
    cropedImage->setPixmap(QPixmap::fromImage(_m_image));
    cropedImage->show();


    QPushButton *save = new QPushButton("Enregistrer");
    connect(save, SIGNAL(clicked()), this, SLOT(save()));
    connect(save, SIGNAL(clicked()), this, SLOT(close()));
    QPushButton *cancel = new QPushButton("Annuler");
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));

    // This layout will contain the buttons for save and quit.
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(save);
    buttonLayout->addWidget(cancel);

    // This layout will contain the image and the previous layout.
    QVBoxLayout *primeLayout = new QVBoxLayout;
    primeLayout->addWidget(widget);
    primeLayout->addLayout(buttonLayout);

    this->setLayout(primeLayout);
    this->show();
}
