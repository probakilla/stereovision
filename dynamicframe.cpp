#include "dynamicframe.h"


DynamicFrame::DynamicFrame(QWidget *parent, int sizeCorners) : QWidget(parent), sizeCorner{sizeCorners}
{
    setMouseTracking(true);

    tlc = new MovableButton(this,sizeCorners);
    trc = new MovableButton(this,sizeCorners);
    blc = new MovableButton(this,sizeCorners);
    brc = new MovableButton(this,sizeCorners);
}

/**
 * @brief DynamicFrame::moveCorners makes all corners move and stay alligned to each others
 * Only one corner should be moving during a call
 * @param pos position of the mouse at the moment of the moving operation
 */
void DynamicFrame::moveCorners(QPoint pos)
{
    if(tlc->isMoving())
    {
        tlc->move(pos);
        trc->move(trc->x(), pos.y());
        blc->move(pos.x(),  brc->y());
    }
    if(trc->isMoving())
    {
        trc->move(pos);
        tlc->move(tlc->x(), pos.y());
        brc->move(pos.x(), brc->y());
    }
    if(blc->isMoving())
    {
        blc->move(pos);
        tlc->move(pos.x(), tlc->y());
        brc->move(brc->x(), pos.y());
    }
    if(brc->isMoving())
    {
        brc->move(pos);
        blc->move(blc->x(),pos.y());
        trc->move(pos.x(), trc->y());

    }
}

void DynamicFrame::mouseMoveEvent(QMouseEvent * event)
{
    //QWidget::mouseMoveEvent(event);

    moveCorners(event->pos() - QPoint(sizeCorner,sizeCorner)/2);

     //GET A QSTRING FROM A stringstream
    /*std::stringstream ss;
    ss << tlc->x();
    text->setText( QString(ss.str().c_str()));*/
}

void DynamicFrame::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

}