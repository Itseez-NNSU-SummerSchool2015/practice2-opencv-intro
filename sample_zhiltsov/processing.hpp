#pragma once

#include <opencv2/core/core.hpp>

struct FilterType {
	enum {
		Grayscale,
		Pixelize,
		Canny,
		Blur
	};
};

class Processing
{
 public:
    void processFrame(const cv::Mat& src, cv::Mat& dst);

	void setFilterType(int filterType);
 private:
	cv::Rect region;
	int filterType;

	void pixelize(const cv::Mat& src, cv::Mat& dst);
};
