#include "panorama.h"
#include <numeric>

panorama::panorama(){}
panorama::~panorama(){}
//function to determine the axial mip and range of axial slices needed to make skeleton
cv::Mat panorama::panoramaSliceSelect(){
    cv::Mat coronalMip = coronalImages[0]->clone();
    for(int i = 0; i<coronalImages.size();i++){
        for(int row = 0;row<coronalMip.rows;row++){
            for(int col =0;col<coronalMip.cols;col++){
                if(coronalImages[i]->at<uchar>(row,col)>=coronalMip.at<uchar>(row,col)){
                    coronalMip.at<uchar>(row,col) =coronalImages[i]->at<uchar>(row,col);
                }
            }
        }
    }
    cv::Scalar mean,stddev;
    cv::meanStdDev(coronalMip,mean,stddev);
    cv::threshold(coronalMip,coronalMip,mean[0]+2*stddev[0],255,cv::THRESH_BINARY);
    std::vector<float>rowHist(coronalMip.rows,0);
    std::vector<int>ids;
    for(int i =0;i<coronalMip.rows;i++){
        auto count = cv::countNonZero(coronalMip.row(i));
        rowHist[i] = count;
    }
    PeakFinder::findPeaks(rowHist,ids,true,1);
    long sumFreq = std::accumulate(rowHist.begin(),rowHist.end(),0);
    long mulFreq =0;
    for(int i =0;i<rowHist.size();i++){
        mulFreq += i*rowHist[i];
    }
    double Mean = mulFreq/double(sumFreq);
    double sum =0;
    for(int i=0;i<rowHist.size();i++){
        sum += pow((i - Mean),2)*rowHist[i];

    }
    double stdDev = sqrt(sum/sumFreq);
    std::vector<int>row_ids;
    for(int i =0 ;i<ids.size();i++){
        if(i ==0){
            if(abs((ids[0]-ids[1])) > 70){
//                auto it = ids.begin();
//                ids.erase(it);

            }
            else{
                row_ids.push_back(ids[i]);
            }
        }else if( i == ids.size()-1){
            if(abs((ids[ids.size()-1]-ids[ids.size()-2])) > 70){
//                auto it = ids.end();
//                ids.erase(it);

            }
            else{
                row_ids.push_back(ids[i]);
            }
        }
        else{
            if((abs((ids[i]-ids[i-1])) >70)&&(abs((ids[i+1]-ids[i])) >70)){
//                auto it = ids.begin();
//                it +=i;
//                ids.erase(it);
            }
            else{
                row_ids.push_back(ids[i]);
            }
        }

    }
    for(int i =0 ;i<row_ids.size();i++){
        for(int j=0 ;j<coronalMip.cols;j++){
            coronalMip.at<uchar>(row_ids[i],j)=255;
        }
    }
    std::pair<int,int>HighestPeak={row_ids[0],rowHist[row_ids[0]]};
    std::pair<int,int>secondHighestPeak=HighestPeak;
    int H =1;
    if(row_ids.size()>1){
        for(int i =1 ;i<row_ids.size();i++){
            if(rowHist[row_ids[i]] > HighestPeak.second){
                HighestPeak={row_ids[i],rowHist[row_ids[i]]};
            }
        }
        for (int i = 0;i<row_ids.size();i++){
            if((rowHist[row_ids[i]]>= 0.5* HighestPeak.second)  &&  (row_ids[i] != HighestPeak.first))
            {
                secondHighestPeak = {row_ids[i],rowHist[row_ids[i]]};
            }
        }
    }
    else{
        HighestPeak = {row_ids[0],rowHist[row_ids[0]]};
    }
    cv::Mat axialMip = axialImages[HighestPeak.first]->clone();
    for(int i = HighestPeak.first-1.5*stdDev;
        i<((secondHighestPeak.first+2.5*stdDev)>axialImages.size()-1?axialImages.size()-1:(secondHighestPeak.first+2.5*stdDev));
        i++){
        for(int row = 0;row<axialMip.rows;row++){
            for(int col =0;col<axialMip.cols;col++){
                if(axialImages[i]->at<uchar>(row,col)>=axialMip.at<uchar>(row,col)){
                    axialMip.at<uchar>(row,col) =axialImages[i]->at<uchar>(row,col);
                }
            }
        }
    }
    return axialMip;
}

void panorama::SkeletonGenerate(cv::Mat axialMip){
    cv::Mat skeleton = cv::Mat::zeros(axialMip.size(),CV_8UC1);
    cv::Scalar Ma,Sa;
    cv::meanStdDev(axialMip,Ma,Sa);
    cv::threshold(axialMip,axialMip,Ma[0]+2*Sa[0],255,cv::THRESH_BINARY);
    cv::copyMakeBorder(axialMip,axialMip,50,50,50,50,cv::BORDER_CONSTANT,cv::Scalar(0));
    cv::Mat im_floodfill = axialMip.clone();
    cv::floodFill(im_floodfill, cv::Point(0, 0), cv::Scalar(255));
    cv::Mat im_floodfill_inv;
    cv::bitwise_not(im_floodfill, im_floodfill_inv);
    cv::Mat holeFilled = (axialMip | im_floodfill_inv);
    cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(30,30));
    cv::Mat closingImg;
    morphologyEx(holeFilled, closingImg,cv::MORPH_CLOSE, element,cv::Point(-1, -1), 2);

    cv::Mat labelImage(axialMip.size(), CV_32S),dst(axialMip.size(), CV_8UC3);
    int nLabels = connectedComponents(closingImg, labelImage, 8);
    std::vector<cv::Vec3b> colors(nLabels);
    colors[0] = cv::Vec3b(0, 0, 0);//background
    for(int label = 1; label < nLabels; ++label){
        colors[label] = cv::Vec3b(0, 0, 0);
        if(label ==1)
        colors[label] = cv::Vec3b(255, 255, 255);
    }
    for(int r = 0; r < dst.rows; ++r){
        for(int c = 0; c < dst.cols; ++c){
            int label = labelImage.at<int>(r, c);
            cv::Vec3b &pixel = dst.at<cv::Vec3b>(r, c);
            pixel = colors[label];
        }
    }
    cvtColor(dst, dst, cv::COLOR_RGB2GRAY);
    this->Offset(dst);
    cv::Mat blur;
    cv::blur(dst,blur,cv::Size(9,9));
    cv::Mat SkeletonImg;
    cv::ximgproc::thinning (blur, SkeletonImg ,  0);
    for(int row = 50;row<SkeletonImg.rows-50;row++){
        for(int col =50;col<SkeletonImg.cols-50;col++){
            skeleton.at<uchar>(row-50,col-50)=SkeletonImg.at<uchar>(row,col);
        }
    }
    this->skeleton = skeleton;
}

void panorama::Offset(cv::Mat img) {
    std::vector<int>whiteNum;
    for (int y = 0; y < img.cols; y++) {
        int sum = 0;
        for (int x = 0; x < img.rows; x++)
        {
            if (img.at<uchar>(y, x) > 0)
            {
                sum++;
            }
        }
        whiteNum.push_back(sum);
    }
    int avg =0;
    for(int i =0;i<whiteNum.size();i++){
        avg += whiteNum[i];
    }
    avg /= whiteNum.size();
    this->offset =  avg>84?avg:60;
}

void panorama::ctrlPtsCalculate(cv::Mat skeleton){
    std::vector <int >ptsX;
    std::vector <int >ptsY;
    this->curveCtrlX.clear();
    this->curveCtrlY.clear();

    for (int i = 0; i < skeleton.cols; i++)//To loop through all the pixels
    {
        for (int j = 0; j < skeleton.rows; j++)
        {
            if (skeleton.at<uchar>(j, i) == 255)
            {
                ptsX.push_back(i);
                ptsY.push_back(j);
            }
        }
    }

    for (int i = 0; i <= 10; i++) {
        std::vector<int>::iterator csitX = (i* ((ptsX.end() - 1) - ptsX.begin()) / 10) + ptsX.begin();
        std::vector<int>::iterator csitY = (i* ((ptsY.end() - 1) - ptsY.begin()) / 10) + ptsY.begin();
        cv::Point cubicPoint = { *csitX ,  *csitY };
        this->curveCtrlX.push_back(*csitX);
        this->curveCtrlY.push_back(*csitY);
}
}

void panorama::shiftCurve(float shift) {

    tk::spline curve(this->curveCtrlX, this->curveCtrlY, tk::spline::cspline);
    std::vector<double>derivs;
    if(this->curveShiftX.size() != 0){
        this->curveShiftX.clear();
        this->curveShiftY.clear();
    }
    for (int i = 0;i < this->curveCtrlX.size();i++) {

        derivs.push_back(curve.deriv(1, this->curveCtrlX[i]));
        this->curveShiftX.push_back(0);
        this->curveShiftY.push_back(0);
    }
    if (shift - (this->offset / 2) > 0) {
        float shiftValue = (shift - this->offset/2 );
        for (int i = 0;i < this->curveCtrlX.size();i++) {
            this->curveShiftY[i] = this->curveCtrlY[i] - (shiftValue / sqrt(1 + pow(derivs[i], 2)));
            this->curveShiftX[i] = this->curveCtrlX[i] + ((derivs[i] * shiftValue) / sqrt(1 + pow(derivs[i], 2)));
        }
    }
    else if (shift - (this->offset / 2) < 0) {
        float shiftValue = (this->offset/2 - shift);
        for (int i = 0;i < this->curveCtrlX.size();i++) {
            this->curveShiftY[i] = this->curveCtrlY[i] + (shiftValue / sqrt(1 + pow(derivs[i], 2)));
            this->curveShiftX[i] = this->curveCtrlX[i] - ((derivs[i] * shiftValue) / sqrt(1 + pow(derivs[i], 2)));
        }
    }
    else {
        //control points stay the same if not required a shift
        for (int i = 0;i < this->curveCtrlX.size();i++) {
            this->curveShiftX[i] = this->curveCtrlX[i];
            this->curveShiftY[i] = this->curveCtrlY[i];
        }
    }

}

void panorama::panoramaProjection(float shiftValue){
    panorama::shiftCurve(shiftValue);
    tk::spline curve(this->curveShiftX,this->curveShiftY,tk::spline::cspline);
    int colNum =this->curveShiftX[this->curveShiftX.size()-1] - this->curveShiftX[0] +2;
    this->panoramaImage =cv::Mat(cv::Size(colNum,axialImages.size()),CV_8UC1);
    for(int i=0;i<axialImages.size();i++){
        int col=0;
        for(int j =this->curveShiftX[0];j<this->curveShiftX[this->curveShiftX.size()-1];j++){
            if(j < 0 || j> axialImages[i]->cols-1 || curve(j)<0 ||curve(j)>axialImages[i]->rows-1){
                this->panoramaImage.at<uchar>(i,col) = 0;
            }
            else{
                this->panoramaImage.at<uchar>(i,col) = axialImages[i]->at<uchar>(curve(j),j);
            }
            col++;

        }
    }
    
}

cv::Mat panorama::serialProjection(int id){
    tk::spline curve(this->curveCtrlX,this->curveCtrlY,tk::spline::cspline);
    cv::Mat serial(axialImages.size(),203*3,CV_8UC1);
    float slope = curve.deriv(1,id);
    int point_x =id, point_y = curve(id);
    int b = -1*slope*point_x+point_y;
    for(int i =0;i<axialImages.size();i++){
    int col=0;
    for(int x =id-100;x<id+100;x++){
       int y = slope*x+b;
       if(x < 0 || x> axialImages[i]->cols-1 || y<0 ||y>axialImages[i]->rows-1){
           serial.at<uchar>(i,col)=0;
       }
       else
            serial.at<uchar>(i,col) = axialImages[i]->at<uchar>(y,x);
       col++;

    }
    }
    float slope_2 = curve.deriv(1,id+5);
    int point_x_2 =id+5, point_y_2 = curve(id+5);
    int b_2 = -1*slope_2*point_x_2+point_y_2;
    for(int i =0;i<axialImages.size();i++){
    int col=203;
    for(int x =id+5-100;x<id+5+100;x++){
       int y = slope_2*x+b_2;
       if(x < 0 || x> axialImages[i]->cols-1 || y<0 ||y>axialImages[i]->rows-1){
           serial.at<uchar>(i,col)=0;
       }
       else
            serial.at<uchar>(i,col) = axialImages[i]->at<uchar>(y,x);
       col++;

    }
    }
    float slope_3 = curve.deriv(1,id+20);
    int point_x_3 =id+20, point_y_3 = curve(id+20);
    int b_3 = -1*slope_3*point_x_3+point_y_3;
    for(int i =0;i<axialImages.size();i++){
    int col=406;
    for(int x =id+20-100;x<id+20+100;x++){
       int y = slope_3*x+b_3;
       if(x < 0 || x> axialImages[i]->cols-1 || y<0 ||y>axialImages[i]->rows-1){
           serial.at<uchar>(i,col)=0;
       }
       else
            serial.at<uchar>(i,col) = axialImages[i]->at<uchar>(y,x);
       col++;
    }
    }
    return serial;
}

void panorama::processPanorama(){
    cv::Mat axialImage = this->panoramaSliceSelect();
    this->SkeletonGenerate(axialImage);
    this->ctrlPtsCalculate(this->skeleton);
}
