#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), picture{}
{
    this->showMaximized();
    this->setMinimumSize(800,600);

    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    picture = new QLabel(widget);
    picture->show();
    pixMap = new QPixmap();

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

// --- TENTATIVE DE CROP NE FONCTIONNE PAS CORRECTEMENT --------
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        x1 = event->x();
        y1 = event->y();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        x2 = event->x();
        y2 = event->y();
        QRect rect(x1, y1, x2 - x1, y2 - y1);
        QPixmap cropped = pixMap->copy(rect);
        cropped.save("cropped_image.jpg");
        QImageReader reader("cropped_image.jpg");
        *pixMap = QPixmap::fromImage(reader.read());
        set_pictures_to_full_size();
    }
}

