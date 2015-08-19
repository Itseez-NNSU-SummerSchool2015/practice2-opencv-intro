#pragma once

#include <opencv2/core/core.hpp>
#include <time.h>
class Processing
{
 public:
	enum FilterType
	{
	   MEDIAN,
	   CVT_CONVERT_GRAY,
	   PIXELIZED,
	   CANNY
	};
    void processFrame(const cv::Mat& src, cv::Mat& dst,  FilterType filter);

};
