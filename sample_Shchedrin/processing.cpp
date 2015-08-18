#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>//TODO:delete

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, cv::Rect region)
{
    src.copyTo(dst);

    Mat roi = dst(region);
    if(applyEdges){
        Mat buf;
        cvtColor(roi, buf, CV_RGB2GRAY);
        GaussianBlur(buf, buf, Size(3, 3), 1.6);
        Canny(buf,buf,60,200);
        vector<Mat> rgb;
        rgb.push_back(buf);
        rgb.push_back(buf);
        rgb.push_back(buf);
        merge(rgb, roi);        
    }
    if(applyPixel){
        int pixelSize = 10;
        for(int i = 0; pixelSize * i < roi.rows; i++){
            for(int j = 0; pixelSize * j < roi.cols; j++){
                Rect square(j*pixelSize, i* pixelSize, pixelSize, pixelSize);
                Point center(square.x + square.width/2, square.y + square.height /2);
                Scalar color = mean(roi(square));
                roi(square).setTo(Scalar(0,0,0));
                circle(roi(square), Point(pixelSize/2, pixelSize/2), pixelSize/2, color, -1);
            }
        }
    }
    if(applyGray){
        Mat buf;
        cvtColor(roi, buf, CV_RGB2GRAY);
        vector<Mat> rgb;
        rgb.push_back(buf);
        rgb.push_back(buf);
        rgb.push_back(buf);
        merge(rgb, roi);
    }
    const int kSize = 11;
    if(applyMedian){
        medianBlur(roi, roi, kSize);
    }

    rectangle(dst, region, Scalar(255, 0, 0));
}

Processing::Processing(){
    applyEdges = false;
    applyGray = false;
    applyPixel = false;
}
