#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst,int a,int b)
{
    src.copyTo(dst);

    cv::Rect region(0, 0, a, b);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
