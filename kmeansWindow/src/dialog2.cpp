#include "dialog2.h"
#include "ui_dialog2.h"
#include <QComboBox>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <opencv2/opencv.hpp>

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);

}

QImage Dialog2::imageFromMat(cv::Mat const& src) {
 Q_ASSERT(src.type() == CV_8UC3);
 cv::Mat * mat = new cv::Mat(src.cols,src.rows,src.type());
 cvtColor(src, *mat, cv::COLOR_BGR2RGB);
 return QImage((uchar*)mat->data, mat->cols, mat->rows,QImage::Format_RGB888);
}

void Dialog2::inputImg(cv::Mat &img1, cv::Mat &img2, cv::Mat &img3, cv::Mat &img4 ){
    Dialog2::qimg1 = Dialog2::imageFromMat(img1);
    Dialog2::qimg2 = Dialog2::imageFromMat(img2);
    Dialog2::qimg3 = Dialog2::imageFromMat(img3);
    Dialog2::qimg4 = Dialog2::imageFromMat(img4);

    int w = ui->label->width();
    int h = ui->label->height();

    ui->label->setPixmap(QPixmap::fromImage( Dialog2::qimg1).scaled(w,h,Qt::KeepAspectRatio));
    ui->label_2->setPixmap(QPixmap::fromImage( Dialog2::qimg2).scaled(w,h,Qt::KeepAspectRatio));
    ui->label_3->setPixmap(QPixmap::fromImage( Dialog2::qimg3).scaled(w,h,Qt::KeepAspectRatio));
    ui->label_4->setPixmap(QPixmap::fromImage( Dialog2::qimg4).scaled(w,h,Qt::KeepAspectRatio));

}

Dialog2::~Dialog2()
{

    delete ui;
}

void Dialog2::on_buttonBox_accepted()
{
    Dialog2::outImage = ui->comboBox->currentText();
}


