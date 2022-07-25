#include "statistics.hpp"
statistics::statistics(){}

statistics::~statistics(){}

void statistics::calculateMeanStdDev(){

    long sumFreq = std::accumulate(histogram.begin(),histogram.end(),0);
    long long mulFreq =0;
    this->mean =0;
    this->stdDev =0;
    for(int i =0;i<histogram.size();i++){
//        std::cout<<"Freq of "<<i<<" is "<<histogram[i]<<std::endl;
        mulFreq += i*histogram[i];
    }
    this->mean = mulFreq/double(sumFreq);
//    std::cout<<"mean is "<<this->mean<<std::endl;
    double sum =0;
    for(int i=0;i<histogram.size();i++){
        sum += pow((i - mean),2)*histogram[i];

    }
    this->stdDev = sqrt(sum/sumFreq);
//    std::cout<<"stdDev is "<<this->stdDev<<std::endl;

}

void statistics::calculateHist(std::vector<std::shared_ptr<cv::Mat>>& axialImages){
    if(this->histogram.size() ==0){
        for(int i=0;i<256;i++){
                this->histogram.push_back(0);
        }
    }
    else{
        this->histogram.clear();
        for(int i=0;i<256;i++){
                this->histogram.push_back(0);
        }
    }
    for(int i =0;i<axialImages.size();i++){
        for(int row =0;row<axialImages[0]->rows;row++)
            for(int col =0;col<axialImages[0]->cols;col++)
                this->histogram[axialImages[i]->at<uchar>(row,col)]++;
    }

}

void statistics::Reset(){
    if(this->histogram.size() !=0){
        this->histogram.clear();
        for(int i=0;i<256;i++){
            this->histogram.push_back(0);
        }
    }
    else{
        for(int i=0;i<256;i++){
            this->histogram.push_back(0);
        }
    }
    this->mean =0;
    this->stdDev =0;
}
