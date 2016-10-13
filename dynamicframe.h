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

class DynamicFrame : public QWidget
{
    Q_OBJECT

public:
    DynamicFrame( QWidget *parent = nullptr, int sizeCorners = 30 );
    enum class Action{ stopped, following, cropping, recropping };

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
    void moveCorners( QPoint pos );
    void setSelectionRectangle();

    int sizeCorner;

    MovableButton* tlc;
    MovableButton* trc;
    MovableButton* blc;
    MovableButton* brc;
    QRubberBand* rect;

    Action state;

};

#endif // DYNAMICFRAME_H
