#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialog2.h"
#include<QPixmap>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    QPixmap pix("E:/gp/secwindow/canal.jpeg");
    int w = ui->labelw->width();
    int h = ui->labelw->height();
    ui->labelw->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::on_pushButton_clicked()
{
    cv::Mat canal = cv::imread("E:/gp/secwindow/canal.jpeg");

    Dialog2 second;
    second.inputImg(canal, canal, canal, canal );
    second.setModal(true);
    second.exec();
    QMessageBox msg;
    msg.setText(second.outImage);
    msg.exec();
}

