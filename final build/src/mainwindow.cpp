#include "mainwindow.h"
#include "./ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resetView();

    connect(ui->actionOpenFolder, SIGNAL(triggered()), this, SLOT(openFolder()));


    QObject::connect(ui->axialSlider, &QSlider::valueChanged,
            this, &MainWindow::axialSliderCtrl);
    QObject::connect(ui->coronalSlider, &QSlider::valueChanged,
            this, &MainWindow::coronalSliderCtrl);
    QObject::connect(ui->sagittalSlider, &QSlider::valueChanged,
            this, &MainWindow::sagittalSliderCtrl);
    QObject::connect(ui->panoramaSlider, &QSlider::valueChanged,
            this, &MainWindow::panoramaSliderCtrl);

    QObject::connect(ui->axialMinScreen, &QPushButton::clicked, this, &MainWindow::axialMinView);
    QObject::connect(ui->axialMaxScreen, &QPushButton::clicked, this, &MainWindow::axialMaxView);

    QObject::connect(ui->coronalMinScreen, &QPushButton::clicked, this, &MainWindow::coronalMinView);
    QObject::connect(ui->coronalMaxScreen, &QPushButton::clicked, this, &MainWindow::coronalMaxView);

    QObject::connect(ui->sagittalMinScreen, &QPushButton::clicked, this, &MainWindow::sagittalMinView);
    QObject::connect(ui->sagittalMaxScreen, &QPushButton::clicked, this, &MainWindow::sagittalMaxView);

    QObject::connect(ui->curveMinScreen, &QPushButton::clicked, this, &MainWindow::curveMinView);
    QObject::connect(ui->curveMaxScreen, &QPushButton::clicked, this, &MainWindow::curveMaxView);

    QObject::connect(ui->serialMinScreen, &QPushButton::clicked, this, &MainWindow::serialMinView);
    QObject::connect(ui->serialMaxScreen, &QPushButton::clicked, this, &MainWindow::serialMaxView);

    QObject::connect(ui->panoramaMinScreen, &QPushButton::clicked, this, &MainWindow::panoramaMinView);
    QObject::connect(ui->panoramaMaxScreen, &QPushButton::clicked, this, &MainWindow::panoramaMaxView);

    QObject::connect(ui->volumeMinScreen, &QPushButton::clicked, this, &MainWindow::volumeMinView);
    QObject::connect(ui->volumeMaxScreen, &QPushButton::clicked, this, &MainWindow::volumeMaxView);

    QObject::connect(ui->configRotateButton, &QPushButton::clicked, this, &MainWindow::configImagesRotation);
    QObject::connect(ui->configApplyRotateButton, &QPushButton::clicked, this, &MainWindow::ApplyRotationConfigs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::axialSliderCtrl(){
    int id = ui->axialSlider->value();
    MainWindow::axialViewCtrl(id);
}
void MainWindow::coronalSliderCtrl(){
    int id = ui->coronalSlider->value();
    MainWindow::coronalViewCtrl(id);
}
void MainWindow::sagittalSliderCtrl(){
    int id = ui->sagittalSlider->value();
    MainWindow::sagittalViewCtrl(id);
}
void MainWindow::panoramaSliderCtrl(){
    int id = ui->panoramaSlider->value();
    MainWindow::panoramaViewCtrl(id);
}

void MainWindow::axialViewCtrl(int id){
    sceneManager.sceneAxial = new Scenez(this);
    QPixmap axialPixmap= ASM::cvMatToQPixmap(*this->imageManager.axialImages[id]);
    sceneManager.sceneAxial->addPixmap(axialPixmap);
    QRectF rc(QPoint(0, 0), axialPixmap.size());
    ui->axialView->fitInView(rc, Qt::KeepAspectRatio);
    ui->axialView->setScene(sceneManager.sceneAxial);
}
void MainWindow::coronalViewCtrl(int id){
    sceneManager.sceneCoronal = new Scenez(this);
    QPixmap coronalPixmap= ASM::cvMatToQPixmap(*this->imageManager.coronalImages[id]);
    sceneManager.sceneCoronal->addPixmap(coronalPixmap);
    QRectF rc(QPoint(0, 0), coronalPixmap.size());
    ui->coronalView->fitInView(rc, Qt::KeepAspectRatio);
    ui->coronalView->setScene(sceneManager.sceneCoronal);
}
void MainWindow::sagittalViewCtrl(int id){
    sceneManager.sceneSagittal = new Scenez(this);
    QPixmap sagittalPixmap= ASM::cvMatToQPixmap(*this->imageManager.sagittalImages[id]);
    sceneManager.sceneSagittal->addPixmap(sagittalPixmap);
    QRectF rc(QPoint(0, 0), sagittalPixmap.size());
    ui->sagittalView->fitInView(rc, Qt::KeepAspectRatio);
    ui->sagittalView->setScene(sceneManager.sceneSagittal);
}
void MainWindow::panoramaViewCtrl(int id){
    this->panorama.panoramaProjection(id);
    sceneManager.scenePanorama = new Scenez(this);
    QPixmap panoramaPixmap= ASM::cvMatToQPixmap(this->panorama.panoramaImage);
    sceneManager.scenePanorama->addPixmap(panoramaPixmap);
    QRectF rc(QPoint(0, 0), panoramaPixmap.size());
    ui->panoramaView->fitInView(rc, Qt::KeepAspectRatio);
    ui->panoramaView->setScene(sceneManager.scenePanorama);
}

void MainWindow::setViews(){
    MainWindow::axialViewCtrl(0);
    MainWindow::coronalViewCtrl(0);
    MainWindow::sagittalViewCtrl(0);
    MainWindow::panoramaViewCtrl(this->panorama.offset/2);
}
void MainWindow::setSliders(){
    ui->axialSlider->setRange(0,this->imageManager.axialImages.size()-1);
    ui->axialSlider->setValue(0);
//    std::cout<<this->imageManager.axialImages.size()<<std::endl;

    ui->coronalSlider->setRange(0,this->imageManager.coronalImages.size()-1);
    ui->coronalSlider->setValue(0);
//    std::cout<<this->imageManager.coronalImages.size()<<std::endl;

    ui->sagittalSlider->setRange(0,this->imageManager.sagittalImages.size()-1);
    ui->sagittalSlider->setValue(0);
//    std::cout<<this->imageManager.sagittalImages.size()<<std::endl;
    ui->panoramaSlider->setRange(0,this->panorama.offset);
    ui->panoramaSlider->setValue(this->panorama.offset/2);
}

void MainWindow::axialMinView(){
    ui->axialWidget->hide();
}
void MainWindow::axialMaxView(){
    ui->axialWidget->show();
    ui->coronalWidget->hide();
    ui->sagittalWidget->hide();
    ui->curveWidget->hide();
    ui->serialWidget->hide();
    ui->panoramaWidget->hide();
    ui->volumeWidget->hide();
}
void MainWindow::axialShowView(){
    ui->axialWidget->show();
}

void MainWindow::coronalMinView(){
    ui->coronalWidget->hide();
}
void MainWindow::coronalMaxView(){
    ui->coronalWidget->show();
    ui->axialWidget->hide();
    ui->sagittalWidget->hide();
    ui->curveWidget->hide();
    ui->serialWidget->hide();
    ui->panoramaWidget->hide();
    ui->volumeWidget->hide();
}
void MainWindow::coronalShowView(){
    ui->coronalWidget->show();
}

void MainWindow::sagittalMinView(){
    ui->sagittalWidget->hide();
}
void MainWindow::sagittalMaxView(){
    ui->sagittalWidget->show();
    ui->axialWidget->hide();
    ui->coronalWidget->hide();
    ui->curveWidget->hide();
    ui->serialWidget->hide();
    ui->panoramaWidget->hide();
    ui->volumeWidget->hide();
}
void MainWindow::sagittalShowView(){
    ui->axialWidget->show();
}

void MainWindow::curveMinView(){
    ui->curveWidget->hide();
}
void MainWindow::curveMaxView(){
    ui->curveWidget->show();
    ui->axialWidget->hide();
    ui->coronalWidget->hide();
    ui->sagittalWidget->hide();
    ui->serialWidget->hide();
    ui->panoramaWidget->hide();
    ui->volumeWidget->hide();
}
void MainWindow::curveShowView(){
    ui->curveWidget->show();
}

void MainWindow::serialMinView(){
    ui->serialWidget->hide();
}
void MainWindow::serialMaxView(){
    ui->serialWidget->show();
    ui->axialWidget->hide();
    ui->coronalWidget->hide();
    ui->sagittalWidget->hide();
    ui->curveWidget->hide();
    ui->panoramaWidget->hide();
    ui->volumeWidget->hide();
}
void MainWindow::serialShowView(){
    ui->serialWidget->show();
}

void MainWindow::panoramaMinView(){
    ui->panoramaWidget->hide();
}
void MainWindow::panoramaMaxView(){
    ui->panoramaWidget->show();
    ui->axialWidget->hide();
    ui->coronalWidget->hide();
    ui->sagittalWidget->hide();
    ui->curveWidget->hide();
    ui->serialWidget->hide();
    ui->volumeWidget->hide();
}
void MainWindow::panoramaShowView(){
    ui->panoramaWidget->show();
}

void MainWindow::volumeMinView(){
    ui->volumeWidget->hide();
}
void MainWindow::volumeMaxView(){
    ui->volumeWidget->show();
    ui->axialWidget->hide();
    ui->coronalWidget->hide();
    ui->sagittalWidget->hide();
    ui->curveWidget->hide();
    ui->serialWidget->hide();
    ui->panoramaWidget->hide();
}
void MainWindow::volumeShowView(){
    ui->volumeWidget->show();
}

void MainWindow::resetView(){
    ui->axialWidget->show();
    ui->coronalWidget->show();
    ui->sagittalWidget->show();
    ui->curveWidget->show();
    ui->serialWidget->show();
    ui->panoramaWidget->show();
    ui->volumeWidget->show();
    ui->configWidget->hide();
}

void MainWindow::configView(){
    ui->configWidget->show();
    ui->axialWidget->hide();
    ui->coronalWidget->hide();
    ui->sagittalWidget->hide();
    ui->curveWidget->hide();
    ui->serialWidget->hide();
    ui->panoramaWidget->hide();
    ui->volumeWidget->hide();

    cv::Mat mipImage = this->imageManager.imageProcess.MIP(this->imageManager.coronalImages);
    QGraphicsScene* configScene = new QGraphicsScene;
    QPixmap axialPixmap= ASM::cvMatToQPixmap(mipImage);
    configScene->addPixmap(axialPixmap);
    QRectF rc(QPoint(0, 0), axialPixmap.size());
    ui->configView->fitInView(rc, Qt::KeepAspectRatio);
    ui->configView->setScene(configScene);
    ui->configRotateButton->setEnabled(true);
}
void MainWindow::configImagesRotation(){
    this->rotationConfigVal++;
    this->rotationConfigVal %=2;
}
void MainWindow::ApplyRotationConfigs(){
    if (this->rotationConfigVal !=0){
        this->imageManager.OrientaionCorrection();
    }
    MainWindow::processPanorama();
    MainWindow::resetView();
    MainWindow::setSliders();
    MainWindow::setViews();
}

void MainWindow::openFolder(){
    folderPath= QFileDialog::getExistingDirectory(this,
            tr("Open Folder"),
            "C:\\");

    QDir  Directory = folderPath;
    QStringList DICOMS = Directory.entryList(QStringList()<<"*.dcm", QDir::Files);
    DICOM_Names.clear();
    foreach (QString Dicom, DICOMS){
        DICOM_Names.push_back(Dicom.toUtf8().constData());
    }
    SI::natural::sort(DICOM_Names);
    Full_Path_Names.clear();
    for(int i =0;i<DICOM_Names.size();i++){
         Full_Path_Names.push_back(folderPath.toStdString() + "//" + DICOM_Names[i]);
    }
    MainWindow::processImages();
    MainWindow::configView();
}

void MainWindow::processImages(){
    this->imageManager.processImages(this->folderPath.toStdString(),this->Full_Path_Names);
}
void MainWindow::processPanorama(){
    this->panorama.axialImages = this->imageManager.axialImages;
    this->panorama.coronalImages = this->imageManager.coronalImages;
    this->panorama.processPanorama();
}

