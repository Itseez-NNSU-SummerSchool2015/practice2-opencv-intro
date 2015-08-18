#pragma once

#include <opencv2/core/core.hpp>

class Processing
{
 public:
    void processFrame(const cv::Mat& src, cv::Mat& dst, int size_of_filter_window);
};
