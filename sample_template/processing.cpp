#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, int t, FilterType filter)
{
    src.copyTo(dst);

    cv::Rect region(src.rows/4 + t / 5, src.cols/4 + t / 5, src.rows/2, src.cols/2);
    Mat roi = dst(region);

    const int kSize = 11;
	switch(filter)
	{
	case MEDIAN:
		{
			medianBlur(roi, roi, kSize);
			
			break;
		}
	case CVT_CONVERT_GRAY:
		{
			Mat gray_patch;
			cvtColor(roi, gray_patch, cv::COLOR_BGR2GRAY);
			cvtColor(gray_patch, roi, cv::COLOR_GRAY2BGR);
			
			break;
		}
	case PIXELIZED:
		{
			GaussianBlur(roi, roi, Size( 19, 19 ), 0, 0 );
			 
			 break;
		}
		case CANNY:
		{
			Mat gray_patch;
			cvtColor(roi, gray_patch, cv::COLOR_BGR2GRAY);
			Canny(gray_patch, gray_patch, 0, 59);
			cvtColor(gray_patch, roi, cv::COLOR_GRAY2BGR);
			
			break;
		}
		default:
  break;
}
    rectangle(dst, region, Scalar(255, 0, 0));
}
