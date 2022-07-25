#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

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
using std::vector;
using std::string;
using std::shared_ptr;
using cv::Mat;
class ImageProcess
{
public:
    ImageProcess();
    ~ImageProcess();
    void LoadImages(vector<string>& Path ,std::vector<DicomImage*>DicomImages, vector<shared_ptr<Mat>>& axialImages);
    void ArtifactReduction(vector<shared_ptr<Mat>>& axialImages,int threshold,int slope,int intercept);
    void GrayTransform(vector<shared_ptr<Mat>>& axialImages);
    void MultiPlanarReconstruction(vector<shared_ptr<Mat>>& axialImages    ,
                                   vector<shared_ptr<Mat>>& coronalImages  ,
                                   vector<shared_ptr<Mat>>& sagittalImages );
    void ReverseImages(vector<shared_ptr<Mat>>& axialImages    ,
                       vector<shared_ptr<Mat>>& coronalImages  ,
                       vector<shared_ptr<Mat>>& sagittalImages );
    cv::Mat MIP(vector<shared_ptr<Mat>>& axialImages);
};

#endif // IMAGEPROCESS_H
