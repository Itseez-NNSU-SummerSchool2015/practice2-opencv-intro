#include "processing.hpp"
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);
	int height = rand() % (src.rows / 2);
	int width = rand() %  (src.cols / 2);

	Rect region(width, height, src.cols / 2, src.rows / 2);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
