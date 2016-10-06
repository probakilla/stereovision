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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);

public slots:
    void msgBoxAbout();
    void openFile();


private:
    void set_pictures_to_full_size();

    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    QAction *actionOpenFile;
    QAction *actionQuit;
    QAction *actionAbout;

    QPixmap *pixMap;
    QLabel *picture;
};

#endif // MAINWINDOW_H
