#ifndef DYNAMICFRAME_H
#define DYNAMICFRAME_H

#include <iostream>
#include <sstream>

#include <QWidget>
#include <QResizeEvent>
#include <QLabel>
#include <QPixmap>

#include <movablebutton.h>

class DynamicFrame : public QWidget
{
    Q_OBJECT

public:
    DynamicFrame(QWidget *parent = nullptr, int sizeCorners = 30);

private:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    void moveCorners(QPoint pos);


    MovableButton* tlc;
    MovableButton* trc;
    MovableButton* blc;
    MovableButton* brc;

    int sizeCorner;

};

#endif // DYNAMICFRAME_H
