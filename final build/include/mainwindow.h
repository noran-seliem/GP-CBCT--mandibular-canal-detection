#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<imageManager.h>
#include<SceneManger.h>
#include<opencv2/opencv.hpp>
#include<cvQTconvert.h>
#include<naturalSort.h>
#include<QFileDialog>
#include<panorama.h>
#include<PeakFinder.h>
#include<imageprocess.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ImageManager imageManager;
    SceneManager sceneManager;
    panorama panorama;
    std::vector<std::string> DICOM_Names;
    std::vector<std::string> Full_Path_Names;
    QString folderPath;
    int rotationConfigVal =0;

public slots:
    void processImages();
    void processPanorama();

    void axialSliderCtrl();
    void coronalSliderCtrl();
    void sagittalSliderCtrl();
    void panoramaSliderCtrl();

    void axialViewCtrl(int id);
    void coronalViewCtrl(int id);
    void sagittalViewCtrl(int id);
    void panoramaViewCtrl(int id);

    void axialMinView();
    void axialShowView();
    void axialMaxView();

    void coronalMinView();
    void coronalShowView();
    void coronalMaxView();

    void sagittalMinView();
    void sagittalShowView();
    void sagittalMaxView();

    void curveMinView();
    void curveShowView();
    void curveMaxView();

    void serialMinView();
    void serialShowView();
    void serialMaxView();

    void panoramaMinView();
    void panoramaShowView();
    void panoramaMaxView();

    void volumeMinView();
    void volumeShowView();
    void volumeMaxView();

    void resetView();

    void configView();
    void configImagesRotation();
    void ApplyRotationConfigs();

    void setViews();
    void setSliders();
    void openFolder();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
