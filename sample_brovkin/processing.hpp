#pragma once

#include <opencv2/core/core.hpp>

enum FilterType
	{
		MEDIAN,
		CVT_CONVERT_GRAY,
		PIXELIZED,
		CANNY
	};
class Processing
{
 public:
    void processFrame(const cv::Mat& src, cv::Mat& dst, FilterType filter);
};
