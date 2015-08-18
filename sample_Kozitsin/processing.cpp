#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <stdlib.h>
#include <time.h>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
	srand(time(NULL));

    src.copyTo(dst);

    int x = rand() % src.rows;
    int y = rand() % src.cols;

    while (x > 400 || y > 400)
    {
    	x = rand() % src.rows - 1;
    	y = rand() % src.cols - 1;
    }

    cv::Rect region(x, y, 100, 100);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
