#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
int step=0;
int expansion=0;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, FilterType filter)
{
    src.copyTo(dst);

	if ((3*src.rows/4+step+expansion>src.rows)||(3*src.cols/4+step+expansion>src.cols)) {
		step=0;
		expansion=0;
	}
	
	cv::Rect region(src.rows/4+step, src.cols/4+step, src.rows/2+expansion, src.cols/2+expansion);

    Mat roi = dst(region);

    const int kSize = 11;
	switch (filter)
	{
	case (MEDIAN): {
		medianBlur(roi, roi, kSize);
		break; }
	case (CVT_CONVERT_GRAY): {
		Mat gray;
		cvtColor(roi, gray, CV_RGB2GRAY);
		Mat grays[3] = {gray, gray, gray};
		merge(grays, 3, roi);
		break; }
	case (CANNY): {
		Mat edges;
		cvtColor(roi, edges, CV_RGB2GRAY);
		GaussianBlur(edges, edges, Size(7,7), 1.5);
		Canny(edges, edges, 0, 50);
		Mat edges_arr[3] = {edges, edges, edges};
		merge(edges_arr, 3, roi);
		break;
				  }
	case (PIXELIZED_ROUND): {
		Mat res = Mat::zeros(roi.size(), CV_8UC3);
		Mat cir = Mat::zeros(roi.size(), CV_8UC1);
		int bsize = 10;

		for (int i = 0; i < roi.rows; i += bsize)
		{
			for (int j = 0; j < roi.cols; j += bsize)
			{
				Rect rect = Rect(j, i, bsize, bsize) & 
					Rect(0, 0, roi.cols, roi.rows);

				Mat sub_dst(res, rect);
				sub_dst.setTo(mean(roi(rect)));

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
		normalize(cir_32f, cir_32f, 0, 1, cv::NORM_MINMAX);

		Mat dst_32f;
		res.convertTo(dst_32f, CV_32F);

		std::vector<cv::Mat> channels;
		split(dst_32f, channels);
		for (int i = 0; i < channels.size(); ++i)
			channels[i] = channels[i].mul(cir_32f);

		merge(channels, dst_32f);
		dst_32f.convertTo(roi, CV_8U);

		break;
					  }
	case (PIXELIZED_SQUARE): {
		Mat res = Mat::zeros(roi.size(), CV_8UC3);
		Mat cir = Mat::zeros(roi.size(), CV_8UC1);
		int bsize = 10;

		for (int i = 0; i < roi.rows; i += bsize)
		{
			for (int j = 0; j < roi.cols; j += bsize)
			{
				Rect rect = Rect(j, i, bsize, bsize) & 
					Rect(0, 0, roi.cols, roi.rows);

				Mat sub_dst(roi, rect);
				sub_dst.setTo(mean(roi(rect)));
			}
		}
		break;
							 }
	}

    rectangle(dst, region, Scalar(255, 0, 0));
	step++;
	expansion++;
}
