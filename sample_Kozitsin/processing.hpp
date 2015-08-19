#pragma once

#include <opencv2/core/core.hpp>

class Processing
{
 public:
    void processFrameMedian(const cv::Mat& src, cv::Mat& dst);
    void processFrameGrey(const cv::Mat& src, cv::Mat& dst);
    void processFrameCanny(const cv::Mat& src, cv::Mat& dst);
    void processFramePixel(const cv::Mat& src, cv::Mat& dst);
};
