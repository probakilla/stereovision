#include "dynamicframe.h"


DynamicFrame::DynamicFrame(QWidget *parent, int sizeCorners) : QWidget(parent), sizeCorner{sizeCorners}, state{Action::stopped}
{
    setMouseTracking(true);
    setAutoFillBackground(true);

    tlc = new MovableButton(this,sizeCorners);
        tlc->hide();
    trc = new MovableButton(this,sizeCorners);
        trc->hide();
    blc = new MovableButton(this,sizeCorners);
        blc->hide();
    brc = new MovableButton(this,sizeCorners);
        brc->hide();

    rect = new QRubberBand(QRubberBand::Rectangle, this);
    QGraphicsColorizeEffect *e = new QGraphicsColorizeEffect(rect);
    e->setColor(QColor("black"));
    rect->setGraphicsEffect(e);
    rect->resize(sizeCorners,sizeCorners);
    rect->hide();
}

void DynamicFrame::launch()
{
    state = Action::following;
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

void DynamicFrame::setSelectionRectangle()
{
    int x = (tlc->x() < trc->x() ? tlc->x() : trc->x());
    int y = (tlc->y() < blc->y() ? tlc->y() : blc->y());
    int w = abs(tlc->x() - trc->x());
    int h = abs(tlc->y() - blc->y());

    rect->setGeometry(x, y, w+sizeCorner, h+sizeCorner);
}

void DynamicFrame::mouseMoveEvent(QMouseEvent * event)
{
    if( state == Action::cropping || state == Action::recropping)
    {
        moveCorners(event->pos() - QPoint(sizeCorner,sizeCorner)/2);
        setSelectionRectangle();
    }
}

void DynamicFrame::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if( state == Action::following)
    {
        tlc->move(event->pos());
            tlc->show();
        trc->move(event->pos());
            trc->show();
        blc->move(event->pos());
            blc->show();
        brc->move(event->pos());
            brc->setMoving(true);
            brc->show();

        rect->show();
        state = Action::cropping;
    }
}

void DynamicFrame::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(state == Action::cropping)
    {
        brc->setMoving(false);
        state = Action::recropping;
        grabKeyboard();
    }
}

void DynamicFrame::keyPressEvent ( QKeyEvent * event )
{
    QWidget::keyPressEvent( event );
    if( (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && state == Action::recropping)
    {
        releaseKeyboard();
        tlc->hide();
        trc->hide();
        blc->hide();
        brc->hide();
        rect->hide();
        state = Action::stopped;
        int x = (tlc->x() < trc->x() ? tlc->x() : trc->x());
        int y = (tlc->y() < blc->y() ? tlc->y() : blc->y());
        int w = abs(tlc->x() - trc->x());
        int h = abs(tlc->y() - blc->y());

        emit cropped(QRect(x, y, w+sizeCorner, h+sizeCorner));
    }
}

void DynamicFrame::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

}
