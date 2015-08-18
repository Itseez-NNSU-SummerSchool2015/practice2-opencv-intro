#include "processing.hpp"

#include <time.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);

	int a = clock()/100;
	cv::Rect region((a*a)%(src.rows/2 - 1), a%(src.cols/2 - 1), src.rows/2, src.cols/2);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
