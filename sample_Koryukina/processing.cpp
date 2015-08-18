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
			cvtColor(roi, roi, COLOR_BGR2GRAY);
			//GaussianBlur (roi,roi, Size(7,7));
			break;
		}
		case PIXELIZED:
		{

			break;
		}
		case CANNY:
		{
			blur(roi, roi,Size(3,3));
			//Canny( detected_edges, detected_edges, lowThreshold, lowThreshold
			//Canny(roi,
			break;
		}
		default:
		{
			break;
		}
	}

    rectangle(dst, region, Scalar(255, 0, 0));
}
