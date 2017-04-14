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
#include <QVBoxLayout>
#include <QSignalMapper>

#include <dynamicframe.h>
#include <imageprocessor.h>

#include <sstream>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void resizeEvent(QResizeEvent *event);
    void dispProcessedImage();

public slots:
    void msgBoxAbout();
    void openFile();
    void openRightFile();
    void saveName(int pix);
    void crop( QRect area);
    void splitImageInHalf();
    void blur();
    void canny();
    void sobel();
    void initCalib();
    void dispMap();
    void showKP();
    void featMatch();
    void test();

private:
    void set_pictures_to_full_size();

    QWidget *widget;

    QPixmap *pixMap;
    QLabel *picture;
    DynamicFrame* dFrame;
    imageprocessor *i;

    QPixmap *pixMapDivided;
    QLabel *pictureDivided;

    bool split;

    /// Constantes symboliques.
    const int MIN_SIZE_X = 600;
    const int MIN_SIZE_Y = 800;
};

#endif // MAINWINDOW_H
