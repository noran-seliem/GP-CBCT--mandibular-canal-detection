#ifndef STATISTICS_H
#define STATISTICS_H
#include<vector>
#include<numeric>
#include <opencv2/opencv.hpp>

class statistics{
    public:
        double mean=0;
        double stdDev=0;
        std::vector<int>histogram;
    public:
        statistics();
        ~statistics();
        void calculateMeanStdDev();
        void calculateHist(std::vector<std::shared_ptr<cv::Mat>>& axialImages);
        void Reset();
};
#endif
