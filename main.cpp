#include "mainwindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>


class myLabel: public QLabel
{

  public:
    myLabel()
    {
      this->setAlignment(Qt::AlignCenter);

      //Default Label Value
      this->setText("No Value");

      //set MouseTracking true to capture mouse event even its key is not pressed
      this->setMouseTracking(true);
    };
    ~ myLabel(){};

    void mouseMoveEvent ( QMouseEvent * event )
    {
      //Show x and y coordinate values of mouse cursor here
      this->setText("X:"+QString::number(event->x())+"-- Y:"+QString::number(event->y()));
    };

};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QMainWindow *window= new QMainWindow();
    QWidget *centralWidget = new QWidget(window);
    myLabel* CoordinateLabel = new myLabel();
    QHBoxLayout* layout = new QHBoxLayout(centralWidget);
    layout->addWidget(CoordinateLabel);
    w.show();
    window->show();

    return a.exec();
}
