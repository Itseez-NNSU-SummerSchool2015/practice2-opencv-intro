#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>
#include <math.h>
using namespace cv;
#define PI 3.1415926

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
	srand((unsigned)time(NULL));
	int h, w;
	w = src.rows/8+rand()%(src.rows/8-1);
	h = src.rows/8+rand()%(src.cols/8-1);
    src.copyTo(dst);
	static double diff = 0, dir=0.01;
	if(diff<0||diff>2*PI) dir*=-1;
	diff+=dir;
	int x = src.rows/2+src.rows/4*cos(diff);
	int y = src.cols/2+src.cols/4*sin(diff);
    cv::Rect region(x-w, y-h, w, h);
    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));
}
