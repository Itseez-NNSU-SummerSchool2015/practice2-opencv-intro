#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, FilterType filter)
{
    src.copyTo(dst);
	RNG rng;
	rng.state = time(NULL);
	cv::Rect region(rng.uniform(0, src.rows/2), rng.uniform(0,src.cols/2), src.rows/2, src.cols/2);
	
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
			Mat gray(roi);
			cvtColor(gray, roi, COLOR_BGR2GRAY);
			cvtColor(roi, gray, COLOR_GRAY2BGR);
			break;
		}
		case PIXELIZED:
		{

			break;
		}
		case CANNY:
		{
			Mat gray(roi);
			cvtColor(gray, roi, COLOR_BGR2GRAY);
			blur(gray, gray, Size(3, 3));
			Canny(gray, roi, 10, 100, 3);
			cvtColor(roi, gray, COLOR_GRAY2BGR);

			break;
		}
		default:
		{
			break;
		}
	}

    rectangle(dst, region, Scalar(255, 0, 0));
}
