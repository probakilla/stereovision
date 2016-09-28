#include "mainwindow.h"


#define MINIMUM_HEIGHT 100
#define MINIMUM_WIDTH 200

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumHeight(MINIMUM_HEIGHT);
    setMinimumWidth(MINIMUM_WIDTH);
    lbImage = new QLabel(this);
    setCentralWidget(lbImage);
    menuBar = new QMenuBar(this);

    menuFile = new QMenu("Fichier");
    menuHelp = new QMenu("Help");
    actionOpenFile = menuFile->addAction("Ouvrir");
    actionQuit = menuFile->addAction("Quit");
    menuBar->addMenu(menuFile);
    actionAbout = menuHelp->addAction("About");
    menuBar->addMenu(menuHelp);
    setMenuBar(menuBar);

    connect(actionAbout, SIGNAL(triggered()), this, SLOT(msgBoxAbout()));
    connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{

}

// test git

void MainWindow::resizeEvent(QResizeEvent *){
    //lbImage->setPixmap(lbImage->pixmap()->scaled(this->size(), Qt::KeepAspectRatio));
}

void MainWindow::msgBoxAbout(){
    QMessageBox msgBox;
    msgBox.setText("This is the application developped for the 1st TP.");
    msgBox.exec();
}

void MainWindow::openFile(){
    /*QMessageBox mBoxFileName;
    mBoxFileName.setText(QFileDialog::getOpenFileName(this, tr("OuvrirFichier"), "/home"));
    mBoxFileName.exec();*/

    imgReader = new QImageReader(QFileDialog::getOpenFileName(this, tr("Open Image"), "/home", tr("Images (*.png *.jpg *.bmp)")));
    lbImage->setPixmap(QPixmap::fromImageReader(imgReader));
    lbImage->setFixedSize(lbImage->pixmap()->size());
    lbImage->pixmap()->scaled(lbImage->pixmap()->size(), Qt::KeepAspectRatio);
    MainWindow::resize(lbImage->size().width(), lbImage->size().height());
}
