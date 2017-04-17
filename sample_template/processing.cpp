#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);
	float x=0, y=0;
	int xSize=0, ySize=0;
	x=std::rand()%src.rows/2+src.rows/16;
	y=std::rand()%src.cols/2+src.cols/16;
	xSize=std::rand()%src.rows/8+src.rows/16;
	ySize=std::rand()%src.cols/8+src.cols/16;
	cv::Rect region(x, y, xSize, ySize);
	Mat roi = dst(region);
	const int kSize = 11;
	medianBlur(roi, roi, kSize);
	rectangle(dst, region, Scalar(255, 0, 0));
}