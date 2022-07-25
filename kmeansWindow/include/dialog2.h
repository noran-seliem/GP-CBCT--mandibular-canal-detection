#ifndef DIALOG2_H
#define DIALOG2_H

#include <QDialog>

#include <opencv2/opencv.hpp>


namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = nullptr);
    ~Dialog2();


    QString outImage;
    void inputImg(cv::Mat &img1, cv::Mat &img2, cv::Mat &img3, cv::Mat &img4 );
    static QImage imageFromMat(cv::Mat const& src);


private slots:
    void on_buttonBox_accepted();



private:
    Ui::Dialog2 *ui;
    QImage qimg1;
    QImage qimg2;
    QImage qimg3;
    QImage qimg4;

};

#endif // DIALOG2_H
