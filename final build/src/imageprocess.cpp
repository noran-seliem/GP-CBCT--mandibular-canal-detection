#include "imageprocess.h"

ImageProcess::ImageProcess()
{

}

ImageProcess::~ImageProcess(){

}

void ImageProcess::LoadImages(vector<string>& Path ,std::vector<DicomImage*>DicomImages, vector<shared_ptr<Mat>>& axialImages){
    DJDecoderRegistration::registerCodecs();
    for(int i =0;i<Path.size();i++){
       DicomImages.push_back(new DicomImage(Path[i].c_str(),0,0,0));
       axialImages.push_back(std::make_shared<cv::Mat>(cv::Mat(int(DicomImages[i]->getWidth())
                                                            ,int(DicomImages[i]->getHeight()),CV_16UC1,
                                                            (short*)DicomImages[i]->getOutputData(16))));
    }
}

void ImageProcess::ArtifactReduction(vector<shared_ptr<Mat>>& axialImages,int threshold,int slope,int intercept){
    for(int i =0;i<axialImages.size();i++){
       for(int j=0;j<axialImages[0]->rows;j++){
           for(int k=0;k<axialImages[0]->cols;k++){
               int HU = axialImages[i]->at<ushort>(j,k)*slope + intercept;
               if(HU >threshold){
                   axialImages[i]->at<ushort>(j,k) =threshold - intercept;
               }
           }
       }
   }
}

void ImageProcess::GrayTransform(vector<shared_ptr<Mat>>& axialImages){
    double max=axialImages[0]->at<ushort>(0,0),min=axialImages[0]->at<ushort>(0,1);
    for(int i =0;i<axialImages.size();i++){
        for(int j=0;j<axialImages[0]->rows;j++){
            for(int k=0;k<axialImages[0]->cols;k++){
                if(axialImages[i]->at<ushort>(j,k)<min)
                    min =axialImages[i]->at<ushort>(j,k);
                else if(axialImages[i]->at<ushort>(j,k)>max)
                    max =axialImages[i]->at<ushort>(j,k);
            }
        }
    }
    for(int i =0;i<axialImages.size();i++){
        for(int j=0;j<axialImages[0]->rows;j++){
            for(int k=0;k<axialImages[1]->cols;k++){
                int pixelValue = axialImages[i]->at<ushort>(j,k);
                int grayvalue = int(((pixelValue-min)/(max-min))*(pow(2,8)-1));
                axialImages[i]->at<ushort>(j,k) = grayvalue;
            }
        }
        axialImages[i]->convertTo(*axialImages[i],CV_8UC1 );
    }
}

void ImageProcess::MultiPlanarReconstruction(vector<shared_ptr<Mat>>& axialImages    ,
                                             vector<shared_ptr<Mat>>& coronalImages  ,
                                             vector<shared_ptr<Mat>>& sagittalImages ){
    coronalImages.clear();
    sagittalImages.clear();
    std::vector<std::shared_ptr<cv::Mat>>sagitialTempImages;
    for (int i = 0;i < axialImages[0]->rows; i++)
    {
        sagitialTempImages.push_back(std::make_shared<cv::Mat>(cv::Mat(axialImages[0]->rows, axialImages.size(),0)));
        coronalImages.push_back(std::make_shared<cv::Mat>(cv::Mat(axialImages.size(), axialImages[0]->rows,0)));
    }

    for (int K = axialImages.size() - 1;K >= 0;K--)
    {
        cv::Mat axialImage = *axialImages[K];

        for (int j = 0; j < axialImage.rows; j++)//To loop through all the pixels
        {
            for (int i = 0; i < axialImage.cols; i++)
            {
                coronalImages[j]->at<uchar>(K, i) = axialImage.at<uchar>(j, i);
                sagitialTempImages[j]->at<uchar>(i, K) = axialImage.at<uchar>(i,j);
            }
        }
    }
    for (int i = 0;i < coronalImages.size();i++)
    {
        //REMOVE IF NOT SAGITAL NO NEED TO ROTATE
        double angle = -90;
        // get rotation matrix for rotating the image around its center in pixel coordinates
        cv::Point2f center((sagitialTempImages[i]->cols - 1) / 2.0, (sagitialTempImages[i]->rows - 1) / 2.0);
        cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
        // determine bounding rectangle, center not relevant
        cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), sagitialTempImages[i]->size(), angle).boundingRect2f();
        // adjust transformation matrix
        rot.at<double>(0, 2) += bbox.width / 2.0 - sagitialTempImages[i]->cols / 2.0;
        rot.at<double>(1, 2) += bbox.height / 2.0 - sagitialTempImages[i]->rows / 2.0;

        sagittalImages.push_back(std::make_shared<cv::Mat>());
        cv::warpAffine(*sagitialTempImages[i], *sagittalImages[i], rot, bbox.size());
        cv::flip(*sagittalImages[i], *sagittalImages[i], 1);
    }
    sagitialTempImages.clear();
}

void ImageProcess::ReverseImages(vector<shared_ptr<Mat>>& axialImages    ,
                                 vector<shared_ptr<Mat>>& coronalImages  ,
                                 vector<shared_ptr<Mat>>& sagittalImages ){
    std::reverse(axialImages.begin(),axialImages.end());
    if(coronalImages.size() == sagittalImages.size()){
        for(int i =0;i<coronalImages.size();i++){
            cv::flip(*coronalImages[i],*coronalImages[i],0);
            cv::flip(*sagittalImages[i],*sagittalImages[i],0);
        }
    }
    else{
        for(int i =0;i<coronalImages.size();i++){
            cv::flip(*coronalImages[i],*coronalImages[i],0);
        }
        for(int i =0;i<sagittalImages.size();i++){
            cv::flip(*sagittalImages[i],*sagittalImages[i],0);
        }
    }
}
cv::Mat ImageProcess::MIP(vector<shared_ptr<Mat>>& images){
    cv::Mat Mip = images[0]->clone();
        for(int i = 0; i<images.size();i++){
            for(int row = 0;row<Mip.rows;row++){
                for(int col =0;col<Mip.cols;col++){
                    if(images[i]->at<uchar>(row,col)>=Mip.at<uchar>(row,col)){
                        Mip.at<uchar>(row,col) =images[i]->at<uchar>(row,col);
                    }
                }
            }
        }
    return Mip;
}
















