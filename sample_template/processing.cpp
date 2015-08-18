#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace cv;

void Processing::pixelize(const cv::Mat& src, cv::Mat& dst)
{
		int n = 40;
		for(int i = 0; i < src.rows - n; i += n)
		{
			for(int j = 0; j < src.cols - n; j += n)
			{
				Rect roi(i, j, n, n);
				blur(src(roi), dst(roi), Size(n, n));
			}
		}

		int deltacol = src.cols % n;
		if (deltacol != 0)
		{
			for(int j = 0; j < src.cols - n; j += n)
				{
					Rect roi(src.cols - deltacol, j, deltacol - 1, n);
					blur(src(roi), dst(roi), Size(deltacol - 1, n));
			}
		}
		int deltarow = src.rows % n;
		if (deltarow != 0)
		{
			for(int j = 0; j < src.rows - n; j += n)
				{
					Rect roi(j, src.rows - deltarow, n, deltarow - 1);
					blur(src(roi), dst(roi), Size(n, deltarow - 1));
			}
		}
		Rect roi(src.cols - deltacol, src.rows - deltarow, deltacol - 1, deltarow - 1);
		blur(src(roi), dst(roi), Size(deltacol - 1, deltarow - 1));




	}
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
			//GaussianBlur(roi, roi, Size( 19, 19 ), 0, 0 );
			 pixelize(roi, roi);
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
