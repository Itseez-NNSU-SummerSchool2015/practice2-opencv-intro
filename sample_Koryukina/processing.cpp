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
			Mat cir = cv::Mat::zeros(roi.size(), CV_8UC1);
			int bsize = 10;

			for (int i = 0; i < roi.rows; i += bsize)
			{
				for (int j = 0; j < roi.cols; j += bsize)
				{
					Rect rect = Rect(j, i, bsize, bsize) & 
									Rect(0, 0, roi.cols, roi.rows);

					Mat sub_dst(roi, rect);
					sub_dst.setTo(cv::mean(roi(rect)));

					circle(
						cir, 
						Point(j+bsize/2, i+bsize/2), 
						bsize/2-1, 
						CV_RGB(255,255,255), -1, CV_AA
					);
				}
			}

			Mat cir_32f;
			cir.convertTo(cir_32f, CV_32F);
			cv::normalize(cir_32f, cir_32f, 0, 1, NORM_MINMAX);

			Mat dst_32f;
			roi.convertTo(dst_32f, CV_32F);

			std::vector<Mat> channels;
			split(dst_32f, channels);
			for (int i = 0; i < channels.size(); ++i)
				channels[i] = channels[i].mul(cir_32f);

			merge(channels, dst_32f);
			dst_32f.convertTo(roi, CV_8U);


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
