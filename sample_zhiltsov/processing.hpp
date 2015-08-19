#pragma once

#include <opencv2/core/core.hpp>


class Processing
{
public:
    enum class FilterType {
        Grayscale,
        Pixelize,
        Canny,
        Blur
    };


    Processing(const FilterType& filterType = FilterType::Blur);

    void processFrame(const cv::Mat& src, cv::Mat& dst);

    void setFilterType(const FilterType& filterType);
private:
    cv::Rect region;
    FilterType filterType;

    void applyPixelizeFilter(const cv::Mat& src, cv::Mat& dst);
    void applyCannyFilter(const cv::Mat& src, cv::Mat& dst);
    void applyGrayscaleFilter(const cv::Mat& src, cv::Mat& dst);
    void applyBlurFilter(const cv::Mat& src, cv::Mat& dst);

    void updateRoiPosition();
};
