#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>
#include <math.h>
using namespace cv;
#define PI 3.1415926

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, FilterType filter = MEDIAN)
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
	int kSize, i=0, j=0;
	cv::Rect rect;
	Mat gray;
	switch (filter){
	case MEDIAN: 
		kSize=11;
		medianBlur(roi, roi, kSize); 
		break;
	case CVT_CONVERT_GRAY: 
		cvtColor(roi, gray, COLOR_BGR2GRAY); cvtColor(gray, roi, COLOR_GRAY2BGR);
		break;
	case PIXELIZED: 
		kSize=3;
		for (i = 0; i < roi.rows; i += kSize)
		{
			for (int j = 0; j < roi.cols; j += kSize)
			{
				rect = cv::Rect(j, i, kSize, kSize) & 
                            cv::Rect(0, 0, roi.cols, roi.rows);

				cv::Mat sub_dst(roi, rect);
				sub_dst.setTo(cv::mean(roi(rect)));
			}
		}
		break;
	case CANNY:
		cvtColor(roi, gray, COLOR_BGR2GRAY); 
		Canny(gray, gray, 0, 50);
		cvtColor(gray,roi,COLOR_GRAY2BGR);
		break;
 
	}
    rectangle(dst, region, Scalar(255, 0, 0));
}
