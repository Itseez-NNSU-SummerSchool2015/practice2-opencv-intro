#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
int step=0;
int expansion=0;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);

	if ((3*src.rows/4+step+expansion>src.rows)||(3*src.cols/4+step+expansion>src.cols)) {
		step=0;
		expansion=0;
	}
	
	cv::Rect region(src.rows/4+step, src.cols/4+step, src.rows/2+expansion, src.cols/2+expansion);

    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
	step++;
	expansion++;
}
