#include "processing.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>

using namespace cv;

Processing::Processing(const FilterType_t& filterType_) :
    filterType(filterType_)
{}

void Processing::setFilterType(const FilterType_t& value)
{
    filterType = value;
}

void Processing::updateRoiPosition(const cv::Mat& src) 
{
    region.x = src.rows/4 + region.x * std::abs(sin((double)clock())) * 0.1;
    region.y = src.cols/4 + region.y * std::abs(cos((double)clock())) * 0.1; 
    
    region.width = src.rows/2 + region.width * std::abs(sin((double)clock())) * 0.1;
    region.height = src.cols/2 + region.height * std::abs(cos((double)clock())) * 0.1;    
}

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst)
{
    src.copyTo(dst);
    Mat roi = dst(region);

    switch (filterType) {
    case FilterType::Blur: 
        applyBlurFilter(roi, roi);
        break;
            
    case FilterType::Grayscale:
        applyGrayscaleFilter(roi, roi);
        break;

    case FilterType::Pixelize:
        applyPixelizeFilter(roi, roi);
        break;

    case FilterType::Canny:
        applyCannyFilter(roi, roi);
        break;

    default: { throw "Unexpected filter type."; }
    }

    rectangle(dst, region, CV_RGB(255, 0, 0));
}

/* 
 * Copy-pasted from: http://opencv-code.com/tutorials/photo-to-colored-dot-patterns-with-opencv/
*/
void Processing::applyPixelizeFilter(const Mat& src, Mat& dst)
{
    Mat cir = cv::Mat::zeros(src.size(), CV_8UC1);
    const int bsize = 10;

    for (int i = 0; i < src.rows; i += bsize)
    {
        for (int j = 0; j < src.cols; j += bsize)
        {
            Rect rect = Rect(j, i, bsize, bsize) & 
                        Rect(0, 0, src.cols, src.rows);

            Mat sub_dst(dst, rect);
            sub_dst.setTo(cv::mean(src(rect)));

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
    normalize(cir_32f, cir_32f, 0, 1, NORM_MINMAX);

    Mat dst_32f;
    dst.convertTo(dst_32f, CV_32F);

    std::vector<Mat> channels;
    split(dst_32f, channels);
    for (int i = 0; i < channels.size(); ++i)
        channels[i] = channels[i].mul(cir_32f);

    merge(channels, dst_32f);
    dst_32f.convertTo(dst, CV_8U);
}

void Processing::applyCannyFilter(const Mat& src, Mat& dst) 
{
    const Size kSize(11, 11);
    GaussianBlur(src, dst, kSize, 5, 10); 

    Mat newRoi = dst.clone();
    cvtColor(dst, newRoi, CV_BGR2GRAY);
    Canny(newRoi, newRoi, 10, 1);

    std::vector<Mat> channels;
    channels.push_back(newRoi);
    channels.push_back(newRoi);
    channels.push_back(newRoi);    
    merge(channels, dst);
}

void Processing::applyGrayscaleFilter(const Mat& src, Mat& dst) 
{
    Mat newRoi = src.clone();
    cvtColor(newRoi, newRoi, CV_BGR2GRAY);

    std::vector<Mat> channels;
    channels.push_back(newRoi);    
    channels.push_back(newRoi);    
    channels.push_back(newRoi);    
    merge(channels, dst);
}

void Processing::applyBlurFilter(const Mat& src, Mat& dst) 
{
    const int kSize = 11;
    medianBlur(src, dst, kSize);
}