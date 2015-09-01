#include "processing.hpp"
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, FilterType filter)
{
	src.copyTo(dst);
		
	cv::Rect region(rand()%src.rows/2, rand()%src.cols/2, src.rows/2, src.cols/2);
	Mat roi = dst(region);

	const int kSize = 11;
	switch (filter){
		case MEDIAN:  medianBlur(roi, roi, kSize);break;
		case CVT_CONVERT_GRAY:	break;
		case PIXELIZED: break;
		case CANNY: Canny(roi,roi,50000,10000);break;
	}
	rectangle(dst, region, Scalar(255, 0, 0));
}
