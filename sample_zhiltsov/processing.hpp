#pragma once

#include <opencv2/core/core.hpp>


class Processing
{
public:
    /* Wanted scoped enum here */
    struct FilterType
    {
        enum 
        {
            Grayscale,
            Pixelize,
            Canny,
            Blur
        };
    };
    typedef int FilterType_t;


    Processing(const FilterType_t& filterType = FilterType::Blur);

    void processFrame(const cv::Mat& src, cv::Mat& dst);

    void setFilterType(const FilterType_t& filterType);
private:
    cv::Rect region;
    FilterType_t filterType;

    void applyPixelizeFilter(const cv::Mat& src, cv::Mat& dst);
    void applyCannyFilter(const cv::Mat& src, cv::Mat& dst);
    void applyGrayscaleFilter(const cv::Mat& src, cv::Mat& dst);
    void applyBlurFilter(const cv::Mat& src, cv::Mat& dst);

    void updateRoiPosition(const cv::Mat& src);
};
