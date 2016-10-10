#include "movablebutton.h"

MovableButton::MovableButton(QWidget *parent, int size) : QLabel(parent), moving{false}
{
    setFixedSize(size,size);
}

void MovableButton::setMoving(bool value)  { moving = value;   }
bool MovableButton::isMoving()             {return moving;     }
void MovableButton::mousePressEvent(QMouseEvent * event)    { event->pos(); moving = true;  }
void MovableButton::mouseReleaseEvent(QMouseEvent * event)  { event->pos(); moving = false; }
void MovableButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
        QRectF rectangle(0,0,width()-1, height()-1 );
        p.setPen(QPen(Qt::black,5));
        p.drawRect(rectangle);
}
