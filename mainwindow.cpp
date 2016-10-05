#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), picture{}
{
    this->showMaximized();

    widget = new QWidget;
    setCentralWidget(widget);
    picture = new QLabel(widget);
    picture->show();
    pixMap = new QPixmap();

    menuFile = new QMenu("Fichier");
    menuTraitement = new QMenu("Traitement");
    menuHelp = new QMenu("Aide");
    menuBar = new QMenuBar(this);
        menuBar->addMenu(menuFile);
        menuBar->addMenu(menuTraitement);
        menuBar->addMenu(menuHelp);
        this->setMenuBar(menuBar);

    actionOpenFile = menuFile->addAction("Ouvrir");
        actionOpenFile->setShortcut(tr("Ctrl+O"));
        connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    actionQuit = menuFile->addAction("Quitter");
        actionQuit->setShortcut(tr("Ctrl+Q"));
        connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    actionAbout = menuHelp->addAction("A propos");
        actionAbout->setShortcut(tr("Ctrl+H"));
        connect(actionAbout, SIGNAL(triggered()), this, SLOT(msgBoxAbout()));
    actionCrop = menuTraitement->addAction("Selection");
}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    set_pictures_to_full_size();
}

void MainWindow::set_pictures_to_full_size()
{
    if( pixMap && picture && !pixMap->isNull())
    {
            picture->setPixmap( pixMap->scaled(this->width(),this->height(), Qt::KeepAspectRatio));
            picture->adjustSize();
    }
}

void MainWindow::msgBoxAbout()
{
    QMessageBox msgBox;
    msgBox.setText("This is the application developped for the 1st TP.");
    msgBox.exec();
}

void MainWindow::openFile()
{
    QFileDialog file_dialog(this);
    QString path = file_dialog.getOpenFileName(this);
    QImageReader reader(path);
    *pixMap = QPixmap::fromImage(reader.read());

    set_pictures_to_full_size();
}

void MainWindow::crop(QMouseEvent *event)
{

    /*QMessageBox test;
    if (event->button() == Qt::LeftButton)
    {
        QPoint p = event->pos();

        QString str = QString::number(p.x());
        test.setText(str);
        test.exec();
    }*/

}
