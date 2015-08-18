#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>
#include <iostream>

using namespace cv;

void Processing::setFilterType(int value)
{
	filterType = value;
}

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);

    region.x = src.rows/4 + region.x * std::abs(sin((float)clock())) * 0.1;
	region.y = src.cols/4 + region.y * std::abs(cos((float)clock())) * 0.1; 
	
	region.width = src.rows/2 + region.width * std::abs(sin((float)clock())) * 0.1;
	region.height = src.cols/2 + region.height * std::abs(cos((float)clock())) * 0.1;

    Mat roi = dst(region);

	switch (filterType) {
	case FilterType::Blur: 
		{
			const int kSize = 11;
			medianBlur(roi, roi, kSize);
			break;
		}
	case FilterType::Grayscale:
		{
			Mat newRoi = roi.clone();
			cvtColor(newRoi, newRoi, CV_BGR2GRAY);
			std::vector<Mat> channels(3);
			channels[0] = newRoi;
			channels[1] = newRoi;
			channels[2] = newRoi;
			merge(channels, roi);
			break;
		}
	case FilterType::Pixelize:
		{
			pixelize(roi, roi);
			break;
		}
	case FilterType::Canny:
		{
			const Size kSize(11, 11);
			GaussianBlur(roi, roi, kSize, 5, 10); 

			Mat newRoi = roi.clone();
			cvtColor(roi, newRoi, CV_BGR2GRAY);

			Canny(newRoi, newRoi, 10, 1);

			std::vector<Mat> channels(3);
			channels[0] = newRoi;
			channels[1] = newRoi;
			channels[2] = newRoi;
			merge(channels, roi);

			break;
		}

	default: { throw "Filter is not specefied."; }
	}

    rectangle(dst, region, Scalar(255, 0, 0));
}

/* 
 * Copy-pasted from: http://opencv-code.com/tutorials/photo-to-colored-dot-patterns-with-opencv/
*/
void Processing::pixelize(const Mat& src, Mat& dst) 
{
    cv::Mat cir = cv::Mat::zeros(src.size(), CV_8UC1);
    int bsize = 10;

    for (int i = 0; i < src.rows; i += bsize)
    {
        for (int j = 0; j < src.cols; j += bsize)
        {
            cv::Rect rect = cv::Rect(j, i, bsize, bsize) & 
                            cv::Rect(0, 0, src.cols, src.rows);

            cv::Mat sub_dst(dst, rect);
            sub_dst.setTo(cv::mean(src(rect)));

            cv::circle(
                cir, 
                cv::Point(j+bsize/2, i+bsize/2), 
                bsize/2-1, 
                CV_RGB(255,255,255), -1, CV_AA
            );
        }
    }

    cv::Mat cir_32f;
    cir.convertTo(cir_32f, CV_32F);
    cv::normalize(cir_32f, cir_32f, 0, 1, cv::NORM_MINMAX);

    cv::Mat dst_32f;
    dst.convertTo(dst_32f, CV_32F);

    std::vector<cv::Mat> channels;
    cv::split(dst_32f, channels);
    for (int i = 0; i < channels.size(); ++i)
        channels[i] = channels[i].mul(cir_32f);

    cv::merge(channels, dst_32f);
    dst_32f.convertTo(dst, CV_8U);
}