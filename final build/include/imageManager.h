#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

#include <dcmtk/dcmimage/dicoimg.h>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmjpeg/djdecode.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include <panorama.h>
#include <statistics.hpp>
#include <DicomTags.hpp>
#include<imageprocess.h>

class ImageManager{
    public:
        std::vector<DicomImage*>DicomImages;
        std::vector<std::shared_ptr<cv::Mat>> axialImages;
        std::vector<std::shared_ptr<cv::Mat>> coronalImages;
        std::vector<std::shared_ptr<cv::Mat>> sagittalImages;
        ImageProcess imageProcess;
        statistics statistics;
        DicomTags  header;

    public:
        ImageManager();
        ~ImageManager();
        void processImages(std::string Dir,std::vector<std::string>& Path);
        void OrientaionCorrection();
        void processPanorama(std::vector<std::shared_ptr<cv::Mat>>& axialImages);
        void Reset();
};
#endif
