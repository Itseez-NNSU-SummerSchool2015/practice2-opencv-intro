#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, cv::Rect region)
{
    src.copyTo(dst);

    Mat roi = dst(region);
    if(applyEdges){
    }
    if(applyPixel){
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
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}

Processing::Processing(){
    applyEdges = false;
    applyGray = true;
    applyPixel = false;
}
