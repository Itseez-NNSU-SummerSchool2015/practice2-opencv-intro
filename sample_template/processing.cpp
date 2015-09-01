#include "processing.hpp"

#include <time.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, FilterType filter)
{
    src.copyTo(dst);

	int a = clock()/100;
	cv::Rect region((a*a)%(src.rows/2 - 1), a%(src.cols/2 - 1), src.rows/2, src.cols/2);
    Mat roi = dst(region);

    const int kSize = 11;

	switch (filter)
	{
	case MEDIAN: { medianBlur(roi, roi, kSize); break; };
	case CVT_CONVERT_GRAY: 
		{ 
			Mat ansver;
			cvtColor(roi,ansver,CV_BGR2GRAY);
			vector<Mat> vec;
			vec.push_back(ansver);
			vec.push_back(ansver);
			vec.push_back(ansver);
			merge(vec,roi);
			break;
		};
	case PIXELIZED: 
		{ 
			break;
		};
	case CANNY: 
		{ 
			break; 
		};
	}

    rectangle(dst, region, Scalar(255, 0, 0));
}
