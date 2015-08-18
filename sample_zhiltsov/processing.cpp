#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>
#include <iostream>
#include <string>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);

    region.x = src.rows/4 + region.x * std::abs(sin((float)clock())) * 0.1;
	region.y = src.cols/4 + region.y * std::abs(cos((float)clock())) * 0.1; 
	
	region.width = src.rows/2 + region.width * std::abs(sin((float)clock())) * 0.1;
	region.height = src.cols/2 + region.height * std::abs(cos((float)clock())) * 0.1;

    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
