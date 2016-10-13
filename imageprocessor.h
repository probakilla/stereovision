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



class ImageProcessor: public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief ImageProcessor  initialize the boolean to false.
     * @param image  Type of element.
     */
    ImageProcessor(QImage image);

    /**
     * @brief crop  Cut the part of _m_image corresponding to rect coordinates and also takes the parallel part.
     * Then puts the two parts together. Save the result in _m_image and set _m_is_croped to 'true'.
     * If _m_is_croped is equals to 'true', this method does nothing.
     * @param  rect This rectangle should not goes through the middle of the image. The coordinates of this QRect
     * will be used to process.
     */
    void crop(QRect const & rect);

    /**
     * @brief affichage  Show _m_image in a window. This window get a button to save the _m_image.
     */
    void affichage();


public slots:
    /**
     * @brief save  Save the _m_image to a choosen path.
     */
    void save();


private:

    QImage _m_image;
    bool _m_is_croped;
};

#endif // IMAGEPROCESSOR_H
