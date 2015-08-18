#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int step = 0;
int size = 0;

int x = 0;
int y = 0;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
	size +=2;
    src.copyTo(dst);
	cv::Rect region;
	
	if (( x + step + src.rows/2 + size < src.rows) 
		&& (y + step + src.cols/2 + size  < src.cols))
	{
		
		cv::Rect r(x + step , y + step, src.rows/2 + size, src.cols/2 + size);
		region =r;

	}
	else
	{
		//size = size - 5;
		if (step != 0)
			step = 0;
		x=0;
		y=0;
		cv::Rect r(x ,y , src.rows/2 + size, src.cols/2 + size );
		region =r;
		size = 0;
	}

    Mat roi = dst(region);

    const int kSize = 11;
    medianBlur(roi, roi, kSize);

    rectangle(dst, region, Scalar(255, 0, 0));

	step = step + 2 ;
	
}
