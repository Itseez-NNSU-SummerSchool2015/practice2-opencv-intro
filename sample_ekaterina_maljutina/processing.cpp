#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);

    cv::Rect region(src.rows/4, src.cols/4, src.rows/2, src.cols/2);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
