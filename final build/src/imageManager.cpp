#include "imageManager.h"

ImageManager::ImageManager(){}

ImageManager::~ImageManager(){}

void ImageManager::processImages(std::string Dir,std::vector<std::string>& Path){
    this->header.loadInfo(Dir);
    this->imageProcess.LoadImages(Path,this->DicomImages,this->axialImages);
    if(this->header.dict.Rescale_Type !="None"){
        this->imageProcess.ArtifactReduction(this->axialImages,3000,this->header.dict.Rescale_Slope,this->header.dict.Rescale_Intercept);
    }
    this->imageProcess.GrayTransform(this->axialImages);
    this->imageProcess.MultiPlanarReconstruction(this->axialImages,this->coronalImages,this->sagittalImages);
    this->statistics.calculateHist(this->axialImages);
    this->statistics.calculateMeanStdDev();
}

void ImageManager::OrientaionCorrection(){
    this->imageProcess.ReverseImages(this->axialImages,this->coronalImages,this->sagittalImages);
}
void ImageManager::processPanorama(std::vector<std::shared_ptr<cv::Mat>>& axialImages){

}
void ImageManager::Reset(){
    for(int i = DicomImages.size()-1; i>=0;i--){
        delete DicomImages[i];
        DicomImages.pop_back();
    }
    axialImages.clear();
    coronalImages.clear();
    sagittalImages.clear();
    this->statistics.Reset();
    this->header.Reset();
}
