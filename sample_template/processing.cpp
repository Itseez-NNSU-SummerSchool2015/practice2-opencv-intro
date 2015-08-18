#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);
	int x=0, y=0;
	x=std::rand()%src.rows/2+src.rows/8;
	y=std::rand()%src.cols/2+src.cols/8;
	cv::Rect region(x, y, src.rows/4, src.cols/4);
	Mat roi = dst(region);
	const int kSize = 11;
	medianBlur(roi, roi, kSize);
	rectangle(dst, region, Scalar(255, 0, 0));
}