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

#include <movablebutton.h>

class DynamicFrame : public QWidget
{
    Q_OBJECT

public:
    DynamicFrame(QWidget *parent = nullptr, int sizeCorners = 30);

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    void moveCorners(QPoint pos);
    void setSelectionRectangle();

    int sizeCorner;

    MovableButton* tlc;
    MovableButton* trc;
    MovableButton* blc;
    MovableButton* brc;
    QRubberBand* rect;

};

#endif // DYNAMICFRAME_H
