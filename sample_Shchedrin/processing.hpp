#pragma once

#include <opencv2/core/core.hpp>

class Processing
{
 public:
    bool applyGray, applyPixel, applyEdges, applyMedian;
    void processFrame(const cv::Mat& src, cv::Mat& dst, cv::Rect region);
    Processing();
};
