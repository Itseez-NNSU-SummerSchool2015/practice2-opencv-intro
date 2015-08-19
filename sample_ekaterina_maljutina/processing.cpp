#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int step = 0;
int size = 0;

int x = 0;
int y = 0;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst,FilterType filter)
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

	switch(filter)
	{
	case MEDIAN: 
		{
			medianBlur(roi, roi, kSize);
			break;
		}
	case CVT_CONVERT_GRAY:
		{
			Mat tmp;
			cvtColor( roi, tmp , CV_BGR2GRAY );

			vector<Mat> ch;
			ch.push_back(tmp);
			ch.push_back(tmp);
			ch.push_back(tmp);

			merge(ch,roi);

			break;
		}
	case PIXELIZED:
		{

			Mat dst = Mat::zeros(roi.size(), CV_8UC3);
			Mat cir = Mat::zeros(roi.size(), CV_8UC1);
			int bsize = 10;

			for (int i = 0; i < roi.rows; i += bsize)
			{
				for (int j = 0; j < roi.cols; j += bsize)
				{
					Rect rect = Rect(j, i, bsize, bsize) & 
									Rect(0, 0, roi.cols, roi.rows);

					Mat sub_dst(dst, rect);
					sub_dst.setTo(mean(roi(rect)));

					circle(
						cir, 
						Point(j+bsize/2, i+bsize/2), 
						bsize/2-1, 
						CV_RGB(255,255,255), -1, CV_AA);
				}
			}

			Mat cir_32f;
			cir.convertTo(cir_32f, CV_32F);
			normalize(cir_32f, cir_32f, 0, 1, NORM_MINMAX);

			Mat dst_32f;
			dst.convertTo(dst_32f, CV_32F);

			std::vector<Mat> channels;
			split(dst_32f, channels);
			for (int i = 0; i < channels.size(); ++i)
				channels[i] = channels[i].mul(cir_32f);

			merge(channels, dst_32f);
			dst_32f.convertTo(dst, CV_8U);

			dst.copyTo(roi);

			break;
		}
	case CANNY:
		{
			Mat tmp;
			Canny(roi,tmp,50,150,3);
			vector<Mat> ch;
			ch.push_back(tmp);
			ch.push_back(tmp);
			ch.push_back(tmp);

			merge(ch,roi);

			break;
		}
	default:
		{
			medianBlur(roi, roi, kSize);
			break;
		}
	}

    rectangle(dst, region, Scalar(255, 0, 0));

	step = step + 2 ;
	
}
