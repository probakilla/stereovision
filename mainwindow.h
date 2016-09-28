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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void msgBoxAbout();
    void openFile();
    void resizeEvent(QResizeEvent *);

private:
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QAction *actionOpenFile;
    QAction *actionQuit;
    QAction *actionAbout;
    QLabel *lbImage;
    QImageReader *imgReader;
};

#endif // MAINWINDOW_H
