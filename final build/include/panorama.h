#ifndef PANORAMA_H
#define PANORAMA_H


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include<spline.h>
#include<PeakFinder.h>
class panorama{
    public:
        std::vector<std::shared_ptr<cv::Mat>> axialImages;
        std::vector<std::shared_ptr<cv::Mat>> coronalImages;
        int offset;
        std::vector<double> curveCtrlX;
        std::vector<double> curveCtrlY;

        std::vector<double> curveShiftX;
        std::vector<double> curveShiftY;

        cv::Mat skeleton;
        cv::Mat panoramaImage;

    public:
        panorama();
        ~panorama();

        cv::Mat panoramaSliceSelect();
        void SkeletonGenerate(cv::Mat);
        void Offset(cv::Mat img);
        void ctrlPtsCalculate(cv::Mat skeleton);
        void shiftCurve(float shift);
        void panoramaProjection(float shiftValue);
        cv::Mat serialProjection(int id);
        void processPanorama();
};
#endif
