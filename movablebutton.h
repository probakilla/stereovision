#ifndef MOVABLEBUTTON_H
#define MOVABLEBUTTON_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

/**
 * @brief The MovableButton class acts like a button, it emits isDown when the user
 * press or release the mouse on it
 */

class MovableButton : public QLabel
{
    Q_OBJECT

    public:
        MovableButton(QWidget *parent = nullptr, int size=30);
        void setMoving(bool value);
        bool isMoving();

    signals:
        void isDown( bool state );

    protected:
        virtual void mousePressEvent(QMouseEvent * event);
        virtual void mouseReleaseEvent(QMouseEvent * event);
        virtual void paintEvent(QPaintEvent * event);

    private:
        bool moving;
};

#endif // MOVABLEBUTTON_H
