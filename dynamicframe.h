#ifndef DYNAMICFRAME_H
#define DYNAMICFRAME_H

#include <iostream>
#include <sstream>
#include <cmath>

#include <QWidget>
#include <QResizeEvent>
#include <QLabel>
#include <QPixmap>
#include <QRubberBand>
#include <QGraphicsColorizeEffect>

#include <movablebutton.h>

/**
 * @brief The DynamicFrame class is a widget with 4 buttons defining a resizable square area
 */
class DynamicFrame : public QWidget
{
    Q_OBJECT

public:
    DynamicFrame( QWidget *parent = nullptr, int sizeCorners = 30 );
    void setSubRect( QRect area );

protected:
    virtual void resizeEvent( QResizeEvent *event );
    virtual void mouseMoveEvent( QMouseEvent *event );
    virtual void mousePressEvent( QMouseEvent *event );
    virtual void mouseReleaseEvent( QMouseEvent *event );
    virtual void keyPressEvent ( QKeyEvent * event );

public slots:
    void launch();

signals:
    void cropped(QRect area);

private:
    enum class Action{ stopped, following, cropping, recropping };
    void moveCorners( QPoint pos );
    void setSelectionRectangle();
    void moveCornerInSubArea(MovableButton* c);

    int _sizeCorner;
    QRect _subArea;

    MovableButton* _tlc;
    MovableButton* _trc;
    MovableButton* _blc;
    MovableButton* _brc;
    QRubberBand* _rect;

    Action _state;

};

#endif // DYNAMICFRAME_H
