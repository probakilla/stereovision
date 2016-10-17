#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), picture{}, dFrame{}
{
    this->showMaximized();
    this->setMinimumSize(800,600);

    widget = new QWidget(this);
    setCentralWidget(widget);

    picture = new QLabel(widget);
    pixMap = new QPixmap();

    dFrame = new DynamicFrame(widget);
    dFrame->setPalette(Qt::transparent);
    connect( dFrame, SIGNAL(cropped(QRect)), this, SLOT(crop(QRect)));

    QMenu* menuFile = menuBar()->addMenu("&Fichier");
    QAction* actionOpenFile = menuFile->addAction("Ouvrir");
    QMenu* menuEdition = menuBar()->addMenu("&Edition");
    QAction* actionDivision = menuEdition->addAction("Diviser l'image en deux");
        actionDivision->setShortcut(tr("Ctrl+D"));
        connect(actionDivision, SIGNAL(triggered()), this, SLOT(diviserImageEnDeux()));
    QMenu* menuHelp = menuBar()->addMenu("&Aide");
        actionOpenFile->setShortcut(tr("Ctrl+O"));
        connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    QAction* actionSaveName = menuFile->addAction("&Enregistrer sous");
        actionSaveName->setShortcut(tr("Ctrl+S"));
        connect(actionSaveName, SIGNAL(triggered()), this, SLOT(saveName()));
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
            picture->setPixmap( pixMap->scaled(centralWidget()->width(),centralWidget()->height(), Qt::KeepAspectRatio));
            i->imageResize(centralWidget()->width(), centralWidget()->height());
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
    QImage image = reader.read();
    i = new imageprocessor(image);
    //i->affichage(label);
    *pixMap = QPixmap::fromImage(image);

    set_pictures_to_full_size();

    dFrame->setSubRect(picture->rect());
}

void MainWindow::saveName()
{
    if( pixMap && picture && !pixMap->isNull())
    {
        pixMap->save(QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString("image.png"), "Images (*.png *.gif *.jpg *.jpeg)"));
    }
}

void MainWindow::crop( QRect area)
{
    if(i->getIs_cropped() == false)
    {
        i->crop(area);
        i->display();
        connect(i->getB_valider(), SIGNAL(clicked()), this, SLOT(dispCrop()));
        set_pictures_to_full_size();
    }
    else
    {
        QMessageBox::warning(this, "Attention", QString::fromUtf8("Cette image a déjà été croppée"));
    }

}

void MainWindow::diviserImageEnDeux(){
    if( pixMap && picture && !pixMap->isNull())
    {
        pixMapDivided = new QPixmap();
       *pixMapDivided = pixMap->copy(pixMap->width()/2, 0, pixMap->width()/2, pixMap->height());
       *pixMap = pixMap->copy(0, 0, pixMap->width()/2, pixMap->height());
        picture->setPixmap(*pixMap);
        divided = new QLabel(widget);
        divided->setPixmap(*pixMapDivided);
        divided->move(pixMap->width()+5, 0);
        divided->show();
    }
}

void MainWindow::dispCrop()
{
    *pixMap = QPixmap::fromImage(i->image());
    set_pictures_to_full_size();
}
