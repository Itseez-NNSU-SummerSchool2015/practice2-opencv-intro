#include "processing.hpp"
#include <stdlib.h>
#include <time.h>

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, Processing::FilterType filter)
{
    src.copyTo(dst);

    int random = rand () % 16 + 2;

    cv::Rect region(src.rows/random, src.cols/random, src.rows/2, src.cols/2);
    Mat roi = dst(region);

    switch (filter)
    {
      case Processing::MEDIAN:
      {
	  const int kSize = 11;
	  medianBlur(roi, roi, kSize);
	  break;
      }
      case Processing::CVT_CONVERT_GRAY:
      {
	  Mat gray;
	  cvtColor(roi, gray, CV_BGR2GRAY);
	  int from_to[] = { 0, 0, 0, 1, 0, 2}; 
	  mixChannels(&gray, 1, & roi, 1, from_to, 3);
	  break;
      }
      case Processing::PIXELIZED: 
      {
	  cv::Mat cir = cv::Mat::zeros(src.size(), CV_8UC1);
	  int bsize = 10;

	  for (int i = 0; i < roi.rows; i += bsize)
	  {
	      for (int j = 0; j < roi.cols; j += bsize)
	      {
	      cv::Rect rect = cv::Rect(j, i, bsize, bsize) & 
                            cv::Rect(0, 0, roi.cols, roi.rows);

	      cv::Mat sub_dst(roi, rect);
	      sub_dst.setTo(cv::mean(src(rect)));

	      cv::circle(cir, cv::Point(j+bsize/2, i+bsize/2), bsize/2-1, CV_RGB(255,255,255), -1, CV_AA );
	      }
	  }
	  break;
      }
      case Processing::CANNY:
      {
	  Mat gray, detected_edges;
	  cvtColor(roi, gray, CV_BGR2GRAY);
	  blur(gray, detected_edges, Size(3,3) );
	  Canny( detected_edges, detected_edges, 30, 30*3,3);
	  roi = Scalar::all(0);
	  int from_to[] = { 0, 0, 0, 1, 0, 2}; 
	  mixChannels(&detected_edges, 1, & roi, 1, from_to, 3);
	  break;
      }
    }

    rectangle(dst, region, Scalar(255, 0, 0));
}
