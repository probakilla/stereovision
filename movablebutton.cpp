#include "movablebutton.h"

MovableButton::MovableButton(QWidget *parent, int size) : QLabel(parent), moving{false}
{
    setPixmap(QPixmap("button.jpg").scaled(size,size));
}

void MovableButton::setMoving(bool value)  { moving = value;   }
bool MovableButton::isMoving()             {return moving;     }
void MovableButton::mousePressEvent(QMouseEvent * event)    { event->pos(); moving = true;  }
void MovableButton::mouseReleaseEvent(QMouseEvent * event)  { event->pos(); moving = false; }

