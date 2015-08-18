#include "processing.hpp"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
int move = 0;
bool isMovingRight = true;

void Processing::processFrame(const cv::Mat& src, cv::Mat& dst, FilterType filter)
{
    cv::Rect region;
    src.copyTo(dst);
    if (isMovingRight)
        if (move >= 50)
        {
            isMovingRight = false;
            cv::Rect reg(src.rows/4 + move, src.cols/4, src.rows/2 + 10, src.cols/2 + 100);
            region = reg;
        }
        else
        {
            cv::Rect reg(src.rows/4 + move, src.cols/4, src.rows/2 + 10, src.cols/2 + 100);
            region = reg;
            move++;
        }
    else
        if (move <= -50)
        {
            isMovingRight = false;
            cv::Rect reg(src.rows/4 + move, src.cols/4, src.rows/2 + 10, src.cols/2 + 100);
            region = reg;
        }
        else
        {
            cv::Rect reg(src.rows/4 + move, src.cols/4, src.rows/2 - 10, src.cols/2 - 100);
            region = reg;
            move--;
        }
    Mat roi = dst(region);
    std::cout << region.x << " " << region.y << " " << region.width << " " << region.height << std::endl;

    const int kSize = 11;
    switch (filter)
    {
    case MEDIAN:
        {
            medianBlur(roi, roi, kSize);
            break;
        }
    case CVT_CONVERT_GRAY:
        {
            Mat gray;
            cvtColor(roi, gray, CV_BGR2GRAY);
            vector<Mat> grays;
            grays.push_back(gray);
            grays.push_back(gray);
            grays.push_back(gray);
            merge(grays, roi);
            break;
        }
    case PIXELIZED:
        {
            cv::Mat dst1 = cv::Mat::zeros(roi.size(), CV_8UC3);
            cv::Mat cir = cv::Mat::zeros(roi.size(), CV_8UC1);
            int bsize = 10;
    
            for (int i = 0; i < roi.rows; i += bsize)
            {
                for (int j = 0; j < roi.cols; j += bsize)
                {
                    cv::Rect rect = cv::Rect(j, i, bsize, bsize) & 
                                    cv::Rect(0, 0, roi.cols, roi.rows);
    
                    cv::Mat sub_dst(dst1, rect);
                    sub_dst.setTo(cv::mean(roi(rect)));
    
                    cv::circle(
                        cir, 
                        cv::Point(j+bsize/2, i+bsize/2), 
                        bsize/2-1, 
                        CV_RGB(255,255,255), -1, CV_AA
                    );
                }
            }
    
            cv::Mat cir_32f;
            cir.convertTo(cir_32f, CV_32F);
            cv::normalize(cir_32f, cir_32f, 0, 1, cv::NORM_MINMAX);
    
            cv::Mat dst_32f;
            dst1.convertTo(dst_32f, CV_32F);
    
            std::vector<cv::Mat> channels;
            cv::split(dst_32f, channels);
            for (int i = 0; i < channels.size(); ++i)
                channels[i] = channels[i].mul(cir_32f);
    
            cv::merge(channels, dst_32f);
            dst_32f.convertTo(roi, CV_8U);
            break;
        }
    case CANNY:
        {
            Mat edges;
            Canny(roi, edges, 60, 120, 3);
            vector<Mat> grays;
            grays.push_back(edges);
            grays.push_back(edges);
            grays.push_back(edges);
            merge(grays, roi);
            break;
        }
    }


    rectangle(dst, region, Scalar(255, 0, 0));
}
