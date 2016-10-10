#include "mainwindow.h"
#include "imageprocessor.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), picture{}, dFrame{}
{
    this->showMaximized();
    this->setMinimumSize(800,600);

    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    picture = new QLabel(widget);
    picture->show();
    pixMap = new QPixmap();

    dFrame = new DynamicFrame(this);
    dFrame->move(30,30);
    dFrame->show();

    QMenu* menuFile = new QMenu("Fichier");

    QMenu* menuHelp = new QMenu("Aide");
    QMenuBar* menuBar = new QMenuBar(this);
        menuBar->addMenu(menuFile);
        menuBar->addMenu(menuHelp);
        this->setMenuBar(menuBar);

    QAction* actionOpenFile = menuFile->addAction("Ouvrir");
        actionOpenFile->setShortcut(tr("Ctrl+O"));
        connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    QAction* actionQuit = menuFile->addAction("Quitter");
        actionQuit->setShortcut(tr("Ctrl+Q"));
        connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    QAction* actionAbout = menuHelp->addAction("A propos");
        actionAbout->setShortcut(tr("Ctrl+H"));
        connect(actionAbout, SIGNAL(triggered()), this, SLOT(msgBoxAbout()));
}


MainWindow::~MainWindow()
{
    delete pixMap;
    delete picture;
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    set_pictures_to_full_size();
    if(dFrame)
        dFrame->resize(event->size());
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
