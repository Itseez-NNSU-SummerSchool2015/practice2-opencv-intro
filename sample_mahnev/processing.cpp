#include "processing.hpp"
#include <cstdlib>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime> 
using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
	srand(time(NULL));
    src.copyTo(dst);
	int line = rand() % src.rows / 2;
	int point = rand() % src.cols / 2;
	
    cv::Rect region( line, point, src.rows/2, src.cols/2);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
