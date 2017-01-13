#include "dynamicframe.h"


DynamicFrame::DynamicFrame(QWidget *parent, int _sizeCorners) : QWidget(parent), _sizeCorner{_sizeCorners}, _state{Action::stopped}
{
    setMouseTracking(true);
    setAutoFillBackground(true);

    _tlc = new MovableButton(this,_sizeCorners);
        _tlc->hide();
    _trc = new MovableButton(this,_sizeCorners);
        _trc->hide();
    _blc = new MovableButton(this,_sizeCorners);
        _blc->hide();
    _brc = new MovableButton(this,_sizeCorners);
        _brc->hide();

    _rect = new QRubberBand(QRubberBand::Rectangle, this);
    QGraphicsColorizeEffect *e = new QGraphicsColorizeEffect(_rect);
    e->setColor(QColor("black"));
    _rect->setGraphicsEffect(e);
    _rect->resize(_sizeCorners,_sizeCorners);
    _rect->hide();
}

/**
 * @brief DynamicFrame::setSubRect confine the dynamic frame to an area of the screen
 * @param area
 */
void DynamicFrame::setSubRect( QRect area )
{
    _subArea = area;
    _subArea.setWidth(_subArea.width()/2);
}

/**
 * @brief DynamicFrame::launch start the cropping action
 */
void DynamicFrame::launch()
{
    _state = Action::following;
}

/**
 * @brief DynamicFrame::moveCorners makes all corners move and stay alligned to each others
 * Only one corner should be moving during a call
 * @param pos position of the mouse at the moment of the moving operation
 */
void DynamicFrame::moveCorners(QPoint pos)
{
    if(_tlc->isMoving())
    {
        _tlc->move(pos);
        _trc->move(_trc->x(), pos.y());
        _blc->move(pos.x(),  _brc->y());
    }
    if(_trc->isMoving())
    {
        _trc->move(pos);
        _tlc->move(_tlc->x(), pos.y());
        _brc->move(pos.x(), _brc->y());
    }
    if(_blc->isMoving())
    {
        _blc->move(pos);
        _tlc->move(pos.x(), _tlc->y());
        _brc->move(_brc->x(), pos.y());
    }
    if(_brc->isMoving())
    {
        _brc->move(pos);
        _blc->move(_blc->x(),pos.y());
        _trc->move(pos.x(), _trc->y());
    }

    moveCornerInSubArea(_tlc);
    moveCornerInSubArea(_trc);
    moveCornerInSubArea(_blc);
    moveCornerInSubArea(_brc);
}

/**
 * @brief DynamicFrame::moveCornerIn_subArea moves the corner c if it is out of the confinemant area
 * @param c
 */
void DynamicFrame::moveCornerInSubArea(MovableButton* c)
{
    if( c->x() < _subArea.x())
        c->move(_subArea.x(), c->y());
    if( c->x()+_sizeCorner > _subArea.x() + _subArea.width())
        c->move(_subArea.x() + _subArea.width() - _sizeCorner, c->y());
    if( c->y() < _subArea.y())
        c->move(c->x(),_subArea.y());
    if( c->y()+_sizeCorner > _subArea.y() + _subArea.height())
        c->move(c->x(), _subArea.y() + _subArea.height() - _sizeCorner);
}

/**
 * @brief DynamicFrame::setSelectionRectangle set the position and size of the frame linking the 4 corners on the screen
 */
void DynamicFrame::setSelectionRectangle()
{
    int x = (_tlc->x() < _trc->x() ? _tlc->x() : _trc->x());
    int y = (_tlc->y() < _blc->y() ? _tlc->y() : _blc->y());
    int w = abs(_tlc->x() - _trc->x());
    int h = abs(_tlc->y() - _blc->y());

    _rect->setGeometry(x, y, w+_sizeCorner, h+_sizeCorner);
}

void DynamicFrame::mouseMoveEvent(QMouseEvent * event)
{
    /*updates the position of the corners and the frame when the mouse is moving and the widget is cropping*/
    if( _state == Action::cropping || _state == Action::recropping)
    {
        moveCorners(event->pos() - QPoint(_sizeCorner,_sizeCorner)/2);
        setSelectionRectangle();
    }
}
void DynamicFrame::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    /*show and place the corners when the user make a first click during a crop*/
    if( _state == Action::following)
    {
        _tlc->move(event->pos());
            _tlc->show();
        _trc->move(event->pos());
            _trc->show();
        _blc->move(event->pos());
            _blc->show();
        _brc->move(event->pos());
            _brc->setMoving(true);
            _brc->show();

        _rect->show();
        _state = Action::cropping;
    }
}

void DynamicFrame::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    /*When the mouse is released, it change the state of the widget and take keyboard inputs to validate the crop*/
    if(_state == Action::cropping)
    {
        _brc->setMoving(false);
        _state = Action::recropping;
        grabKeyboard();
    }
}

void DynamicFrame::keyPressEvent ( QKeyEvent * event )
{
    /*When the user press enter the widget reset and a cropped signal is emmited*/
    QWidget::keyPressEvent( event );
    if( (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && _state == Action::recropping)
    {
        releaseKeyboard();
        _tlc->hide();
        _trc->hide();
        _blc->hide();
        _brc->hide();
        _rect->hide();
        _state = Action::stopped;
        int x = (_tlc->x() < _trc->x() ? _tlc->x() : _trc->x());
        int y = (_tlc->y() < _blc->y() ? _tlc->y() : _blc->y());
        int w = abs(_tlc->x() - _trc->x());
        int h = abs(_tlc->y() - _blc->y());

        emit cropped(QRect(x, y, w+_sizeCorner, h+_sizeCorner));
    }
}

void DynamicFrame::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

}
