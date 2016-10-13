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

    dFrame = new DynamicFrame(widget);
    dFrame->setPalette(Qt::transparent);
    connect( dFrame, SIGNAL(cropped(QRect)), this, SLOT(crop(QRect)));

    QMenu* menuFile = menuBar()->addMenu("&Fichier");

    QMenu* menuHelp = menuBar()->addMenu("&Aide");
    QAction* actionOpenFile = menuFile->addAction("Ouvrir");
        actionOpenFile->setShortcut(tr("Ctrl+O"));
        connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    QAction* actionQuit = menuFile->addAction("Quitter");
        actionQuit->setShortcut(tr("Ctrl+Q"));
        connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    QAction* actionAbout = menuHelp->addAction("A propos");
        actionAbout->setShortcut(tr("Ctrl+H"));
        connect(actionAbout, SIGNAL(triggered()), this, SLOT(msgBoxAbout()));

    // Création de la barre d'outils
    QToolBar *toolBar = addToolBar("Tools");
    QAction* actionCrop = toolBar->addAction("Crop");
        actionCrop->setShortcut(tr("Ctrl+K"));
        connect(actionCrop, SIGNAL(triggered()), dFrame, SLOT(launch()));
    toolBar->addAction(actionCrop);
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
    QString path = file_dialog.getOpenFileName(this, "Ouvrir une image", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    QImageReader reader(path);
    *pixMap = QPixmap::fromImage(reader.read());

    QMenu* menuEdition = menuBar()->addMenu("&Edition");
    QAction* actionDivision = menuEdition->addAction("Diviser l'image en deux");
        actionDivision->setShortcut(tr("Ctrl+D"));
        connect(actionDivision, SIGNAL(triggered()), this, SLOT(diviserImageEnDeux()));

    set_pictures_to_full_size();
}

void MainWindow::crop( QRect area)
{
    std::stringstream ss;
    ss << "x: " << area.x() << std::endl;
    ss << "y: " << area.y() << std::endl;
    ss << "w: " << area.width() << std::endl;
    ss << "h: " << area.height() << std::endl;

    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(ss.str()));
    msgBox.exec();
}
