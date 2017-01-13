#include "movablebutton.h"

MovableButton::MovableButton(QWidget *parent, int size) : QLabel(parent), _moving{false}
{
    setFixedSize(size,size);
}

void MovableButton::setMoving(bool value)  { _moving = value;   }
bool MovableButton::isMoving()             {return _moving;     }
void MovableButton::mousePressEvent(QMouseEvent * event)    { event->pos(); _moving = true;  }
void MovableButton::mouseReleaseEvent(QMouseEvent * event)  { event->pos(); _moving = false; }
/*Paint square frame*/
void MovableButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
        QRectF rectangle(0,0,width()-1, height()-1 );
        p.setPen(QPen(Qt::gray,5));
        p.drawRect(rectangle);
}
