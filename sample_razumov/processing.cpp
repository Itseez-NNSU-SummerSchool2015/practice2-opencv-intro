#include "processing.hpp"
#include <time.h>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);
    time_t timer;
    timer= time(NULL);
    int x;
    x=src.rows/4+sin(timer/10.)*src.rows/4;
    cv::Rect region(int(x), src.cols/4, src.rows/2, src.cols/2);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
