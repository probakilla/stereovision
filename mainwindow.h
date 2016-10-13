#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QLabel>
#include <QImageReader>
#include <QPixmap>
#include <QPoint>
#include <QMouseEvent>
#include <QTextStream>
#include <QWidget>
#include <QRect>
#include <QToolBar>

#include <dynamicframe.h>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void resizeEvent(QResizeEvent *event);

public slots:
    void msgBoxAbout();
    void openFile();
    void diviserImageEnDeux();


private:
    void set_pictures_to_full_size();

    QPixmap *pixMap;
    QLabel *picture;
    DynamicFrame* dFrame;
};

#endif // MAINWINDOW_H
