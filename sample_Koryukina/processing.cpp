#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>
using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);
	RNG rng;
	rng.state = time(NULL);
	cv::Rect region(rng.uniform(0, src.rows/2), rng.uniform(0,src.cols/2), src.rows/2, src.cols/2);
	
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
