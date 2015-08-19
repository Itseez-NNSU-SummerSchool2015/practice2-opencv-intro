#include "processing.hpp"
#include "application.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, FilterType filter)
{
    src.copyTo(dst);

	//added block
	float x=0, y=0;
	int xSize=0, ySize=0;
	x=std::rand()%src.rows/2+src.rows/16;
	y=std::rand()%src.cols/2+src.cols/16;
	xSize=std::rand()%src.rows/8+src.rows/16;
	ySize=std::rand()%src.cols/8+src.cols/16;
	//

	cv::Rect region(x, y, xSize, ySize);
	Mat roi = dst(region);
	const int kSize = 11;

	switch (filter) {
	case MEDIAN: medianBlur(roi, roi, kSize); break;
	case CVT_CONVERT_GRAY: cvtColor(roi, roi, CV_BGR2GRAY); break;
	case PIXELIZED: break;
	case CANNY: 
		cvtColor(roi, roi, CV_BGR2GRAY);
		GaussianBlur(roi, roi, roi.size(), 0, 0);
		Canny(roi, roi, 1, 1);
		break;
	default: std::cout<<"No such filter option"<<std::endl;
	};


	rectangle(dst, region, Scalar(255, 0, 0));
}