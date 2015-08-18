#include "processing.hpp"
#include <stdlib.h>
#include <time.h>

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);


	int random = rand () % 16 + 2;

    cv::Rect region(src.rows/random, src.cols/random, src.rows/2, src.cols/2);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);
	cvtColor(roi, roi, CV_RGB2GRAY, 0);

    rectangle(dst, region, Scalar(255, 0, 0));
}
