#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), picture{}, dFrame{}
{
    this->showMaximized();
    this->setMinimumSize(800,600);

    widget = new QWidget(this);
    setCentralWidget(widget);

    picture = new QLabel(widget);
    pixMap = new QPixmap();

    pixMapDivided = new QPixmap();
    pictureDivided = new QLabel(widget);

    dFrame = new DynamicFrame(widget);
    dFrame->setPalette(Qt::transparent);
    connect( dFrame, SIGNAL(cropped(QRect)), this, SLOT(crop(QRect)));

    QMenu* menuFile = menuBar()->addMenu("&Fichier");
    QAction* actionOpenFile = menuFile->addAction("Ouvrir");

    QMenu* menuEdition = menuBar()->addMenu("&Edition");
    QAction* actionDivision = menuEdition->addAction("Diviser l'image en deux");
        actionDivision->setShortcut(tr("Ctrl+D"));
        connect(actionDivision, SIGNAL(triggered()), this, SLOT(splitImageInHalf()));
    QAction *actionBlur = menuEdition->addAction("Flouter l'image");
        actionBlur->setShortcut(tr("Ctrl+F"));
        connect(actionBlur, SIGNAL(triggered()), this, SLOT(blur()));
    QAction* actionCanny = menuEdition->addAction("Canny");
        actionCanny->setShortcut(tr("Ctrl+C"));
        connect(actionCanny, SIGNAL(triggered()), this, SLOT(canny()));
    QAction* actionSobel = menuEdition->addAction("Sobel");
        actionSobel->setShortcut(tr("Ctrl+B"));
        connect(actionSobel, SIGNAL(triggered()), this, SLOT(sobel()));
    QAction* actionDispMap = menuEdition->addAction("Disparity map");
        actionDispMap->setShortcut(tr("Ctrl+M"));
        connect(actionDispMap, SIGNAL(triggered()), this, SLOT(dispMap()));
    QAction* actionShowKeyPoints = menuEdition->addAction("Show Keypoints");
        actionShowKeyPoints->setShortcut(tr("Ctrl+E"));
        connect(actionShowKeyPoints, SIGNAL(triggered()), this, SLOT(showKP()));
    QAction* actionFeatMatch = menuEdition->addAction("Feature Matching");
        actionFeatMatch->setShortcut(tr("Ctrl+A"));
        connect(actionFeatMatch, SIGNAL(triggered()), this, SLOT(featMatch()));

    QMenu* menuHelp = menuBar()->addMenu("&Aide");
        actionOpenFile->setShortcut(tr("Ctrl+O"));
        connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    /*QAction* actionSaveName = menuFile->addAction("&Enregistrer sous");
        actionSaveName->setShortcut(tr("Ctrl+S"));
        connect(actionSaveName, SIGNAL(triggered()), this, SLOT(saveName()));*/
    QAction* actionQuit = menuFile->addAction("Quitter");
        actionQuit->setShortcut(tr("Ctrl+Q"));
        connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    QAction* actionAbout = menuHelp->addAction("A propos");
        actionAbout->setShortcut(tr("Ctrl+H"));
        connect(actionAbout, SIGNAL(triggered()), this, SLOT(msgBoxAbout()));

    //TODO : Replace the int in the signalMapper by the pixmap/pixmapdivided
    QSignalMapper *signalMapper = new QSignalMapper(widget);
    QMenu* menuImage1 = menuBar()->addMenu("&Image 1");
    QAction* actionSaveName1 = menuImage1->addAction("&Enregistrer sous");
        signalMapper->setMapping(actionSaveName1, 1);
        connect(actionSaveName1, SIGNAL(triggered()), signalMapper, SLOT(map()));

    //TODO : Hide the menu when pixMapDivided is null.
    QMenu* menuImage2 = menuBar()->addMenu("Image 2");
    QAction* actionSaveName2 = menuImage2->addAction("&Enregistrer sous");
        signalMapper->setMapping(actionSaveName2, 2);
        connect(actionSaveName2, SIGNAL(triggered()), signalMapper, SLOT(map()));
    //menuImage2->hide(); doesn't work.

        connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(saveName(int)));


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
    delete pixMapDivided;
    delete pictureDivided;
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
    if( pixMap && picture && !pixMap->isNull() && pixMapDivided->isNull())
    {
            picture->setPixmap( pixMap->scaled(centralWidget()->width(),centralWidget()->height(), Qt::KeepAspectRatio));
            i->imageResize(centralWidget()->width(), centralWidget()->height());
            picture->adjustSize();
    }
    else if( pixMap && picture && !pixMap->isNull() && !pixMapDivided->isNull())
    {
        picture->setPixmap( pixMap->scaled((centralWidget()->width()/2),centralWidget()->height(), Qt::KeepAspectRatio));
        i->imageResize(centralWidget()->width(), centralWidget()->height());
        picture->adjustSize();

        pictureDivided->move(picture->width() +2, 0);
        pictureDivided->setPixmap(pixMapDivided->scaled((centralWidget()->width()/2)-1, centralWidget()->height(), Qt::KeepAspectRatio));
        pictureDivided->adjustSize();
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
    *pixMap = QPixmap::fromImage(image);

    if(!pixMapDivided->isNull()){
        *pixMapDivided = NULL;
        pictureDivided->clear();
    }

    set_pictures_to_full_size();

    split = false;

    dFrame->setSubRect(picture->rect());
}

// TODO : Improve the function so that it takes the pixmap to save as a parameter.
void MainWindow::saveName(int pix)
{
    if( pix == 1 && !pixMap->isNull())
    {
        pixMap->save(QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString("image.png"), "Images (*.png *.gif *.jpg *.jpeg)"));
    }
    if( pix == 2 && !pixMapDivided->isNull())
    {
        pixMapDivided->save(QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString("image.png"), "Images (*.png *.gif *.jpg *.jpeg)"));
    }

}

void MainWindow::crop( QRect area)
{
    if(i->getIsCroped() == false)
    {
        i->crop(area);
        *pixMap = QPixmap::fromImage(i->getProcessedImage());
        set_pictures_to_full_size();
    }
    else
    {
        QMessageBox::warning(this, "Attention", QString::fromUtf8("Cette image a déjà été croppée"));
    }

}

void MainWindow::splitImageInHalf(){
    if( pixMap && picture && !pixMap->isNull())
    {
        i->splitImage();
        *pixMap = QPixmap::fromImage(i->getImage());
        *pixMapDivided = QPixmap::fromImage(i->getImageAlt());

        set_pictures_to_full_size();

        split = true;
    }
}

void MainWindow::blur()
{
    if( pixMap && picture && !pixMap->isNull())
    {
        i->blur();
        this->dispProcessedImage();
    }
}

void MainWindow::canny()
{
    if( pixMap && picture && !pixMap->isNull())
    {
        i->canny();
        this->dispProcessedImage();
    }
}

void MainWindow::sobel()
{
    if( pixMap && picture && !pixMap->isNull())
    {
        i->sobel();
        this->dispProcessedImage();
    }
}

void MainWindow::dispMap()
{
    if(!split)
    {
        splitImageInHalf();
    }
    if( pixMap && picture && !pixMap->isNull() && !pixMapDivided->isNull())
    {
        i->disparity_map();
    }
}

void MainWindow::showKP()
{
    if(pixMap && picture && !pixMap->isNull() && !pixMapDivided->isNull())
    {
        i->showKeyPoints();
    }
}

void MainWindow::featMatch()
{
    if( pixMap && picture && !pixMap->isNull() && !pixMapDivided->isNull())
    {
        i->featureMatching();
    }
}

void MainWindow::dispProcessedImage()
{
    *pixMapDivided = QPixmap::fromImage(i->getProcessedImage());
    pictureDivided->setPixmap(*pixMapDivided);
    pictureDivided->move(pixMap->width()+5, 0);
    pictureDivided->show();

    set_pictures_to_full_size();
}

